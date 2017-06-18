#include "pch.hpp"
#include "processor.hpp"
#include "default_option_parser.hpp"
#include <be/ctable/table.hpp>
#include <be/core/exceptions.hpp>
#include <iostream>

namespace be::cli {
namespace {

///////////////////////////////////////////////////////////////////////////////
template <typename I>
void get_handlers(std::pair<I, I> range, std::vector<Handler*>& dest) {
   dest.clear();
   for (auto it = range.first, end = range.second; it != end; ++it) {
      dest.push_back(it->second);
   }
   std::sort(dest.begin(), dest.end());
}

} // be::cli::()

///////////////////////////////////////////////////////////////////////////////
Processor::Processor()
   : describe_config_(default_describe_config(console_width(std::cout) < 80)),
     option_parser_(default_option_parser),
     maps_valid_(false),
     enable_options_(true),
     verbose_describe_(false),
     context_(nullptr)
{ }

///////////////////////////////////////////////////////////////////////////////
Processor& Processor::operator()(DescribeConfig config) {
   describe_config_ = std::move(config);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
Processor& Processor::operator()(option_parser parser) {
   option_parser_ = std::move(parser);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
Processor& Processor::operator()(Handler handler) {
   handlers_.push_back(std::move(handler));
   maps_valid_ = false;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
bool Processor::options() const {
   return enable_options_;
}

///////////////////////////////////////////////////////////////////////////////
void Processor::options(bool enable_options) const {
   enable_options_ = enable_options;
}

///////////////////////////////////////////////////////////////////////////////
bool Processor::verbose() const {
   return verbose_describe_;
}

///////////////////////////////////////////////////////////////////////////////
void Processor::verbose(bool enable_verbose_describe) const {
   verbose_describe_ = enable_verbose_describe;
}

///////////////////////////////////////////////////////////////////////////////
void Processor::operator()(int argc, char** argv) {
   arg_sequence args;
   args.reserve(argc);
   for (int i = 1; i < argc; ++i) { // skip first argv (program name)
      args.push_back(argv[i]);
   }
   (*this)(args);
}

///////////////////////////////////////////////////////////////////////////////
void Processor::operator()(const arg_sequence& args) {   
   validate_maps_();

   std::vector<Handler*> temp_handlers;
   HandlerContext ctx(*this, args);
   
   for (std::size_t raw = 0; raw < args.size(); ++raw) {
      if (ctx.argument_(raw)) {
         parse_arg_(ctx, temp_handlers);
         ctx.after_argument_();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::describe(std::ostream& os, Id section) const {
   for (auto& s : describe_config_) {
      if (s.id == section) {
         describe_(os, s);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::describe(std::ostream& os, const S& query) const {
   for (auto& section : describe_config_) {
      describe_(os, section, query);
   }
}

///////////////////////////////////////////////////////////////////////////////
const HandlerContext& Processor::context() const {
   return *context_;
}

///////////////////////////////////////////////////////////////////////////////
/// \return true if this argument should cause the argument position to be incremented
void Processor::parse_arg_(HandlerContext& ctx, std::vector<Handler*>& temp_handlers) {

   // raw phase
   ctx.raw_phase_();

   // check specialized raw positional handlers
   get_handlers(raw_positional_handlers_.equal_range((I32)ctx.raw_position()), temp_handlers);
   try_handlers_(temp_handlers, ctx);
   if (!ctx.stop_) {
      // check generic raw positional handlers
      get_handlers(raw_positional_handlers_.equal_range(-1), temp_handlers);
      try_handlers_(temp_handlers, ctx);
   }
   
   if (ctx.after_phase_()) {
      return;
   }

   // option phase
   if (enable_options_) {
      ctx.option_phase_(option_parser_(ctx.argument()));
      
      std::unordered_multimap<S, Handler*>* map;

      if (ctx.option_info_.is_long_option) {
         map = &long_option_handlers_;
      } else {
         map = &short_option_handlers_;
      }

      for (std::size_t i = 0, n = ctx.option_info_.options.size(); i < n; ++i) {
         ctx.option_(i);

         // check specialized option handlers
         get_handlers(map->equal_range(ctx.option()), temp_handlers);
         try_handlers_(temp_handlers, ctx);
         if (!ctx.stop_) {
            // check generic option handlers
            try_handlers_(generic_option_handlers_, ctx);
         }

         ctx.after_option_();
      }

      if (ctx.after_phase_()) {
         return;
      }
   }
   
   // positional phase
   ctx.positional_phase_();

   // check specialized positional handlers
   get_handlers(positional_handlers_.equal_range((I32)ctx.position()), temp_handlers);
   try_handlers_(temp_handlers, ctx);
   if (!ctx.stop_) {
      // check generic positional handlers
      get_handlers(positional_handlers_.equal_range(-1), temp_handlers);
      try_handlers_(temp_handlers, ctx);
   }

   ctx.after_phase_();
}

///////////////////////////////////////////////////////////////////////////////
void Processor::try_handlers_(std::vector<Handler*>& handlers, HandlerContext& ctx) {
   for (Handler* handler : handlers) {
      (*handler)(ctx);
      ctx.after_handler_();
      if (ctx.stop_) {
         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::validate_maps_() {
   if (!maps_valid_) {
      raw_positional_handlers_.clear();
      generic_option_handlers_.clear();
      short_option_handlers_.clear();
      long_option_handlers_.clear();
      positional_handlers_.clear();

      for (Handler& handler : handlers_) {
         for (std::size_t i = 0, n = handler.raw_position_count(); i < n; ++i) {
            raw_positional_handlers_.insert(std::make_pair(handler.raw_position(i), &handler));
         }
         
         if (handler.generic_option_handler()) {
            generic_option_handlers_.push_back(&handler);
         }

         for (std::size_t i = 0, n = handler.short_option_count(); i < n; ++i) {
            short_option_handlers_.insert(std::make_pair(handler.short_option(i), &handler));
         }

         for (std::size_t i = 0, n = handler.long_option_count(); i < n; ++i) {
            long_option_handlers_.insert(std::make_pair(handler.long_option(i), &handler));
         }

         for (std::size_t i = 0, n = handler.position_count(); i < n; ++i) {
            positional_handlers_.insert(std::make_pair(handler.position(i), &handler));
         }
      }
      
      maps_valid_ = true;
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::describe_(std::ostream& os, const DescribeSection& section, const S& query) const {
   ct::Table table(section.config);

   for (auto& handler : handlers_) {
      handler.describe(section.id, table, verbose_describe_, query);
   }

   if (!table.empty()) {
      if (!section.heading.empty()) {
         os << nl << section.heading;
      }
      os << table;
   }
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Processor& proc) {
   proc.describe(os);
   return os;
}

} // be::cli
