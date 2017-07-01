#include "pch.hpp"
#include "handler_context.hpp"
#include "processor.hpp"
#include "option_error.hpp"
#include <be/core/exceptions.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
const Processor& HandlerContext::processor() const {
   return processor_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  The handling phase during which the handler was called.
HandlerContext::handling_phase HandlerContext::phase() const {
   return phase_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  The raw position on the command line of the current argument being
///         processed.
///
/// \details The first argument has raw position 0.
std::size_t HandlerContext::raw_position() const {
   return raw_position_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  The number of positional arguments that have been processed before
///         the current argument was parsed.
///
/// \details Call increment_position(true) from a handler to increment the
///         position index for future arguments.  Note that the position is
///         never incremented more than once for the same argument, and the
///         increment operation does not occur until all handlers have been
///         run for the current argument.
std::size_t HandlerContext::position() const {
   return position_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  The contents of the command line at the current raw_position().
const S& HandlerContext::argument() const {
   return args_[raw_position_];
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::option() const {
   return option_info_.options.at(current_option_);
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::is_short_option() const {
   return !option_info_.is_long_option;
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::is_long_option() const {
   return option_info_.is_long_option;
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::option_prefix() const {
   return option_info_.prefix;
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::option_suffix() const {
   return option_info_.suffix;
}

///////////////////////////////////////////////////////////////////////////////
std::size_t HandlerContext::value_count() const {
   if (!value_count_valid_) {
      value_count_ = option_value_count();

      for (std::size_t i = raw_position_ + 1, n = args_.size(); i < n; ++i) {
         if (!(arg_consumption_[i] & consumed)) {
            ++value_count_;
         }
      }

      value_count_valid_ = true;
   }

   return value_count_;
}

///////////////////////////////////////////////////////////////////////////////
std::size_t HandlerContext::option_value_count() const {
   if (!option_value_count_valid_) {
      option_value_count_ = 0;

      for (auto state : option_value_consumption_) {
         if (!(state & consumed)) {
            ++option_value_count_;
         }
      }

      option_value_count_valid_ = true;
   }

   return option_value_count_;
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::peek_value(std::size_t value_index) {
   return consume_value_(value_index, unconsumed);
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::consume_value(std::size_t value_index) {
   return consume_value_(value_index, consume_after_handler);
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::consume_value_after_option(std::size_t value_index) {
   return consume_value_(value_index, consume_after_option);
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::consume_value_after_phase(std::size_t value_index) {
   return consume_value_(value_index, consume_after_phase);
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::consume_value_after_argument(std::size_t value_index) {
   return consume_value_(value_index, consume_after_argument);
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::is_option(std::size_t value_index) {
   if (value_index < option_value_count()) {
      return false;
   } else {
      auto results = processor_.option_parser_(peek_value(value_index));
      return !results.options.empty();
   }
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::handled(bool is_handled) {
   if (phase_ == option_phase) {
      option_handled_ = is_handled;
   } else {
      arg_handled_ = is_handled;
   }
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::handled() const {
   if (phase_ == option_phase) {
      return option_handled_;
   } else {
      return arg_handled_;
   }
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::stop(bool should_stop) {
   stop_ = should_stop;
   if (should_stop) {
      handled(true);
   }
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::stop() const {
   return stop_;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::stop_after_phase(bool should_stop) {
   stop_after_phase_ = should_stop;
   if (should_stop) {
      handled(true);
   }
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::stop_after_phase() const {
   return stop_after_phase_;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::increment_position(bool increment_position) {
   increment_position_ = increment_position;
   if (increment_position) {
      handled(true);
   }
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::increment_position() const {
   return increment_position_;
}

///////////////////////////////////////////////////////////////////////////////
const S& HandlerContext::consume_value_(std::size_t value_index, consumed_state when) {
   if (value_index < option_value_count()) {
      for (std::size_t i = 0, n = option_info_.values.size(); i < n; ++i) {
         if (!(option_value_consumption_[i] & consumed)) {
            if (value_index == 0) {
               option_value_consumption_[i] |= when;
               return option_info_.values[i];
            } else {
               --value_index;
            }
         }
      }
   } else {
      value_index -= option_value_count();
      for (std::size_t i = raw_position_ + 1, n = args_.size(); i < n; ++i) {
         if (!(arg_consumption_[i] & consumed)) {
            if (value_index == 0) {
               arg_consumption_[i] |= when;
               return args_[i];
            } else {
               --value_index;
            }
         }
      }
   }

   throw FatalTrace(std::make_error_code(std::errc::invalid_argument), "Invalid value index!");
}

///////////////////////////////////////////////////////////////////////////////
HandlerContext::HandlerContext(Processor& processor, const arg_sequence& args)
   : processor_(processor),
     args_(args),
     raw_position_(0),
     position_(0),
     value_count_(0),
     value_count_valid_(false),
     option_value_count_(0),
     option_value_count_valid_(false),
     phase_(raw_positional_phase),
     arg_handled_(false),
     option_handled_(false),
     stop_(false),
     stop_after_phase_(false),
     increment_position_(false),
     current_option_(0)
{
   if (processor.context_) {
      throw FatalTrace(std::make_error_code(std::errc::operation_in_progress), "Argument processing is already in progress!");
   }

   arg_consumption_.resize(args_.size(), unconsumed);
   processor.context_ = this;
}

///////////////////////////////////////////////////////////////////////////////
HandlerContext::~HandlerContext() {
   processor_.context_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
bool HandlerContext::argument_(std::size_t raw_position) {
   raw_position_ = raw_position;
   arg_handled_ = false;
   stop_ = false;
   stop_after_phase_ = false;
   increment_position_ = false;
   return !(arg_consumption_[raw_position] & consumed);
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::raw_phase_() {
   phase_ = raw_positional_phase;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::option_phase_(OptionParseResult option_info) {
   phase_ = option_phase;
   option_info_ = std::move(option_info);
   current_option_ = 0;
   option_value_consumption_.clear();
   option_value_consumption_.resize(option_info_.values.size(), unconsumed);
   value_count_valid_ = false;
   option_value_count_valid_ = false;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::option_(std::size_t current_option) {
   current_option_ = current_option;
   option_handled_ = false;
   stop_ = false;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::positional_phase_() {
   phase_ = positional_phase;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::clear_option_info_() {
   option_info_.options.clear();
   option_info_.values.clear();
   option_value_consumption_.clear();
   current_option_ = 0;
   option_handled_ = false;
   option_value_count_ = 0;
   option_value_count_valid_ = true;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::after_handler_() {
   for (auto& state : arg_consumption_) {
      if (state & consume_after_handler) {
         state = consumed;
         value_count_valid_ = false;
      }
   }

   for (auto& state : option_value_consumption_) {
      if (state & consume_after_handler) {
         state = consumed;
         value_count_valid_ = false;
         option_value_count_valid_ = false;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::after_option_() {
   for (auto& state : arg_consumption_) {
      if (state & consume_after_option) {
         state = consumed;
         value_count_valid_ = false;
      }
   }

   for (auto& state : option_value_consumption_) {
      if (state & consume_after_option) {
         state = consumed;
         value_count_valid_ = false;
         option_value_count_valid_ = false;
      }
   }

   if (!option_handled_) {
      throw OptionError(*this, "Unrecognized option!");
   }
   arg_handled_ = true;
}

///////////////////////////////////////////////////////////////////////////////
/// \return true if subsequent phases should be skipped for this arg
bool HandlerContext::after_phase_() {
   for (auto& state : arg_consumption_) {
      if (state & consume_after_phase) {
         state = consumed;
      }
   }

   value_count_valid_ = false;
   clear_option_info_();

   return stop_after_phase_;
}

///////////////////////////////////////////////////////////////////////////////
void HandlerContext::after_argument_() {
   for (auto& state : arg_consumption_) {
      if (state & consume_after_argument) {
         state = consumed;
      }
   }

   value_count_valid_ = false;

   if (increment_position_) {
      ++position_;
   }

   if (!arg_handled_) {
      throw ArgumentError(*this, "Unhandled argument!");
   }
}

} // be::cli
