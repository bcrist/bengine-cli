#pragma once
#ifndef BE_CLI_PROCESSOR_HPP_
#define BE_CLI_PROCESSOR_HPP_

#include "handler.hpp"
#include "option_parse_result.hpp"
#include "handler_context.hpp"
#include "describe_config.hpp"
#include <unordered_map>
#include <functional>
#include <iosfwd>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class Processor final {
   friend class HandlerContext;
public:
   using arg_sequence = HandlerContext::arg_sequence;
   using option_parser = std::function<OptionParseResult(const S&)>;

   Processor();

   Processor& operator()(DescribeConfig config);
   Processor& operator()(option_parser parser);
   Processor& operator()(Handler handler);

   bool options() const;
   void options(bool enable_options) const;

   bool verbose() const;
   void verbose(bool enable_verbose_describe) const;

   void operator()(int argc, char** argv);
   void operator()(const arg_sequence& args);

   void describe(std::ostream& os, Id section) const;
   void describe(std::ostream& os, const S& query = S()) const;
   
   template <typename SectionPred>
   void describe(std::ostream& os, SectionPred pred) const {
      for (auto& section : describe_config_) {
         if (pred(section.id)) {
            describe_(os, section);
         }
      }
   }

   const HandlerContext& context() const;

private:
   void parse_arg_(HandlerContext& ctx, std::vector<Handler*>& temp_handlers);
   void try_handlers_(std::vector<Handler*>& handlers, HandlerContext& ctx);
   void validate_maps_();
   void describe_(std::ostream& os, const DescribeSection& section, const S& query = S()) const;
   
   DescribeConfig describe_config_;
   std::vector<Handler> handlers_;
   std::unordered_multimap<I32, Handler*> raw_positional_handlers_;
   option_parser option_parser_;
   std::vector<Handler*> generic_option_handlers_;
   std::unordered_multimap<S, Handler*> short_option_handlers_;
   std::unordered_multimap<S, Handler*> long_option_handlers_;
   std::unordered_multimap<I32, Handler*> positional_handlers_;
   bool maps_valid_;
   mutable bool enable_options_;
   mutable bool verbose_describe_;
   HandlerContext* context_;
};

std::ostream& operator<<(std::ostream& os, const Processor& proc);

} // be::cli

#endif
