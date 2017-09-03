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
}

} // be::cli::()

///////////////////////////////////////////////////////////////////////////////
Processor::Processor()
   : describe_config_(default_describe_config(console_width(std::cout) < 80)),
     option_parser_(default_option_parser)
{ }

///////////////////////////////////////////////////////////////////////////////
void Processor::use_option_parser(option_parser parser) {
   option_parser_ = std::move(parser);
}

///////////////////////////////////////////////////////////////////////////////
Processor& Processor::operator()(handler_ptr handler) {
   if (!handler) {
      return *this;
   }

   Handler* h = handler.get();
   handlers_.push_back(std::move(handler));

   for (std::size_t i = 0, n = h->raw_position_count(); i < n; ++i) {
      raw_positional_handlers_.insert(std::make_pair(h->raw_position(i), h));
   }

   if (h->handles_generic_options()) {
      generic_option_handlers_.push_back(h);
   }

   for (std::size_t i = 0, n = h->short_option_count(); i < n; ++i) {
      short_option_handlers_.insert(std::make_pair(h->short_option(i), h));
   }

   for (std::size_t i = 0, n = h->long_option_count(); i < n; ++i) {
      long_option_handlers_.insert(std::make_pair(h->long_option(i), h));
   }

   for (std::size_t i = 0, n = h->position_count(); i < n; ++i) {
      positional_handlers_.insert(std::make_pair(h->position(i), h));
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
void Processor::process(int argc, char** argv) {
   arg_sequence args;
   args.reserve(argc);

   // TODO detect if first arg is program name?

   for (int i = 1; i < argc; ++i) { // skip first argv (program name)
      args.push_back(argv[i]);
   }

   process(args);
}

///////////////////////////////////////////////////////////////////////////////
void Processor::process(const arg_sequence& args) {
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
void Processor::describe(std::ostream& os, bool verbose, Id section) const {
   for (auto& s : describe_config_) {
      if (s.id == section) {
         describe_(os, verbose, s);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::describe(std::ostream& os, bool verbose, const S& query) const {
   for (auto& section : describe_config_) {
      describe_(os, verbose, section, query);
   }
}

///////////////////////////////////////////////////////////////////////////////
/// \return true if this argument should cause the argument position to be incremented
void Processor::parse_arg_(HandlerContext& ctx, std::vector<Handler*>& temp_handlers) {
   // raw phase
   ctx.raw_phase_();

   // check specialized raw positional handlers
   get_handlers(raw_positional_handlers_.equal_range((I32)ctx.raw_position()), temp_handlers);
   try_handlers_(ctx, temp_handlers);
   if (!ctx.stop_) {
      // check generic raw positional handlers
      get_handlers(raw_positional_handlers_.equal_range(-1), temp_handlers);
      try_handlers_(ctx, temp_handlers);
   }

   if (ctx.after_phase_()) {
      return;
   }

   // option phase
   if (ctx.enable_option_handling()) {
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
         try_handlers_(ctx, temp_handlers);
         if (!ctx.stop_) {
            // check generic option handlers
            try_handlers_(ctx, generic_option_handlers_);
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
   try_handlers_(ctx, temp_handlers);
   if (!ctx.stop_) {
      // check generic positional handlers
      get_handlers(positional_handlers_.equal_range(-1), temp_handlers);
      try_handlers_(ctx, temp_handlers);
   }

   ctx.after_phase_();
}

///////////////////////////////////////////////////////////////////////////////
void Processor::try_handlers_(HandlerContext& ctx, std::vector<Handler*>& handlers) {
   for (Handler* handler : handlers) {
      handler->handle(ctx);
      ctx.after_handler_();
      if (ctx.stop_) {
         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void Processor::configure_describe(DescribeConfig config) {
   describe_config_ = std::move(config);
}

///////////////////////////////////////////////////////////////////////////////
void Processor::describe_(std::ostream& os, bool verbose, const DescribeSection& section, const S& query) const {
   ct::Table table(section.config);

   for (auto& handler : handlers_) {
      handler->describe(section.id, table, verbose, query);
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
   proc.describe(os, false);
   return os;
}

} // be::cli
