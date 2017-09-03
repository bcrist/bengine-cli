#pragma once
#ifndef BE_CLI_HANDLER_CONTEXT_HPP_
#define BE_CLI_HANDLER_CONTEXT_HPP_

#include "option_parse_result.hpp"
#include <set>

namespace be::cli {

class Processor;

///////////////////////////////////////////////////////////////////////////////
class HandlerContext final : Immovable {
   enum consumed_state : char {
      unconsumed = 0,
      consumed = 1,
      consume_after_handler = 2,
      consume_after_option = 4,
      consume_after_phase = 8,
      consume_after_argument = 16
   };

   using consumption_map = std::vector<char>;
   friend class Processor;

public:
   using arg_sequence = std::vector<S>;

   enum handling_phase {
      raw_positional_phase,
      option_phase,
      positional_phase
   };

   const Processor& processor() const;

   bool enable_option_handling() const;
   void enable_option_handling(bool enable_options);

   handling_phase phase() const;

   std::size_t raw_position() const;
   std::size_t position() const;

   const S& argument() const;
   const S& option() const;
   bool is_short_option() const;
   bool is_long_option() const;
   const S& option_prefix() const;
   const S& option_suffix() const;

   std::size_t value_count() const;
   std::size_t option_value_count() const;
   const S& peek_value(std::size_t value_index);
   const S& consume_value(std::size_t value_index);
   const S& consume_value_after_option(std::size_t value_index);
   const S& consume_value_after_phase(std::size_t value_index);
   const S& consume_value_after_argument(std::size_t value_index);

   bool is_option(std::size_t value_index);

   void handled(bool is_handled);
   bool handled() const;

   void stop(bool should_stop);
   bool stop() const;

   void stop_after_phase(bool should_stop);
   bool stop_after_phase() const;

   void increment_position(bool increment_position);
   bool increment_position() const;

private:
   const S& consume_value_(std::size_t value_index, consumed_state when);

   HandlerContext(Processor& processor, const arg_sequence& args);

   bool argument_(std::size_t raw_position);

   void raw_phase_();
   void option_phase_(OptionParseResult option_info);
   void option_(std::size_t current_option);
   void positional_phase_();

   void clear_option_info_();

   void after_handler_();
   void after_option_();
   bool after_phase_();
   void after_argument_();

   // invocation-specific state
   Processor& processor_;
   const arg_sequence& args_;
   consumption_map arg_consumption_;
   bool enable_options_;

   // argument-specific state
   std::size_t raw_position_;
   std::size_t position_;

   mutable std::size_t value_count_;
   mutable bool value_count_valid_;

   mutable std::size_t option_value_count_;
   mutable bool option_value_count_valid_;

   handling_phase phase_;

   bool arg_handled_;
   bool option_handled_;
   bool stop_;
   bool stop_after_phase_;
   bool increment_position_;

   // option-specific state
   OptionParseResult option_info_;
   std::size_t current_option_;
   consumption_map option_value_consumption_;
};

} // be::cli

#endif

// renew auto ins