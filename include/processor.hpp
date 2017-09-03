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
   using handler_ptr = std::unique_ptr<Handler>;

   Processor();

   void use_option_parser(option_parser parser);

   Processor& operator()(handler_ptr handler);

   template <typename H>
   Processor& operator()(H&& handler) {
      return (*this)(std::unique_ptr<Handler>(new H(std::move(handler))));
   }

   void process(int argc, char** argv);
   void process(const arg_sequence& args);

   void configure_describe(DescribeConfig config);

   void describe(std::ostream& os, bool verbose, Id section) const;
   void describe(std::ostream& os, bool verbose, const S& query = S()) const;

   template <typename SectionPred>
   void describe(std::ostream& os, bool verbose, SectionPred pred) const {
      for (auto& section : describe_config_) {
         if (pred(section.id)) {
            describe_(os, verbose, section);
         }
      }
   }

private:
   void parse_arg_(HandlerContext& ctx, std::vector<Handler*>& temp_handlers);
   void try_handlers_(HandlerContext& ctx, std::vector<Handler*>& handlers);
   void describe_(std::ostream& os, bool verbose, const DescribeSection& section, const S& query = S()) const;

   DescribeConfig describe_config_;
   std::vector<handler_ptr> handlers_;
   std::unordered_multimap<I32, Handler*> raw_positional_handlers_;
   option_parser option_parser_;
   std::vector<Handler*> generic_option_handlers_;
   std::unordered_multimap<S, Handler*> short_option_handlers_;
   std::unordered_multimap<S, Handler*> long_option_handlers_;
   std::unordered_multimap<I32, Handler*> positional_handlers_;
};

std::ostream& operator<<(std::ostream& os, const Processor& proc);

} // be::cli

#endif
