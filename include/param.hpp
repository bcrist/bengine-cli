#pragma once
#ifndef BE_CLI_PARAM_HPP_
#define BE_CLI_PARAM_HPP_

#include "option_handler_base.hpp"
#include <be/core/exceptions.hpp>
#include <be/core/logging.hpp>
#include <functional>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename F>
class Param final : public OptionHandlerBase<Param<F>> {
public:
   Param(std::initializer_list<S> short_options, std::initializer_list<S> long_options, S value_name, F func = F())
      : OptionHandlerBase<Param<F>>(std::move(short_options), std::move(long_options)),
        value_name_(std::move(value_name)),
        skip_if_no_value_(false),
        allow_default_value_(false),
        allow_option_as_value_(false),
        ignore_extra_values_(false),
        func_(std::move(func))
   { }

   Param<F>& skip_if_no_value(bool enabled) & {
      skip_if_no_value_ = enabled;
      return *this;
   }

   Param<F> skip_if_no_value(bool enabled) && {
      skip_if_no_value_ = enabled;
      return std::move(*this);
   }

   Param<F>& default_value(S default_value = S()) & {
      allow_default_value_ = true;
      default_value_ = std::move(default_value);
      return *this;
   }

   Param<F> default_value(S default_value = S()) && {
      allow_default_value_ = true;
      default_value_ = std::move(default_value);
      return std::move(*this);
   }

   Param<F>& allow_options_as_values(bool enabled) & {
      allow_option_as_value_ = enabled;
      return *this;
   }

   Param<F> allow_options_as_values(bool enabled) && {
      allow_option_as_value_ = enabled;
      return std::move(*this);
   }

   Param<F>& ignore_extra_values(bool enabled) & {
      ignore_extra_values_ = enabled;
      return *this;
   }

   Param<F> ignore_extra_values(bool enabled) && {
      ignore_extra_values_ = enabled;
      return std::move(*this);
   }

   S option_value_name_() const {
      return value_name_;
   }

   bool is_option_value_optional_() const {
      return allow_default_value_;
   }

   virtual void handle(HandlerContext& ctx) override {
      if (this->enabled_ && !this->enabled_()) {
         return;
      }

      if (ctx.value_count() == 0 || !allow_option_as_value_ && ctx.is_option(0)) {
         if (skip_if_no_value_) {
            return;
         }

         if (!allow_default_value_) {
            if (this->throw_on_error_) {
               throw OptionError(ctx, "Option must have a value!");
            } else {
               be_short_warn() << "Ignoring option '" << S(ctx.option()) << "': must specify a value!" | default_log();
            }
         } else if (this->throw_on_error_) {
            try {
               func_(default_value_);
            } catch (const std::runtime_error& e) {
               throw OptionError(ctx, e.what());
            }
         } else {
            func_(default_value_);
         }
      } else if (ctx.option_value_count() > 1 && !ignore_extra_values_) {
         if (this->throw_on_error_) {
            throw OptionError(ctx, "Option can't take multiple values!");
         } else {
            be_short_warn() << "Ignoring option '" << S(ctx.option()) << "': cannot take multiple values!" | default_log();
         }
      } else {
         if (this->throw_on_error_) {
            try {
               func_(ctx.consume_value_after_phase(0));
            } catch (const std::runtime_error& e) {
               throw OptionError(ctx, e.what());
            }
         } else {
            func_(ctx.consume_value_after_phase(0));
         }
      }
      ctx.handled(true);
      ctx.stop_after_phase(true);
   }

private:
   S value_name_;
   S default_value_;
   bool skip_if_no_value_;
   bool allow_default_value_;
   bool allow_option_as_value_;
   bool ignore_extra_values_;
   F func_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename F>
Param<F> param(std::initializer_list<S> short_options,
               std::initializer_list<S> long_options,
               S value_name, F func = F()) {
   return Param<F>(std::move(short_options), std::move(long_options), value_name, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto param(std::initializer_list<S> short_options,
               std::initializer_list<S> long_options,
               S value_name, S& dest) {
   auto func = [&dest](const S& str) {
      dest = str;
   };
   return Param<decltype(func)>(std::move(short_options), std::move(long_options), value_name, std::move(func))
      .extra(ct::Cell() << "Default value: " << color::fg_cyan << dest << color::reset);
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
auto param(std::initializer_list<S> short_options,
           std::initializer_list<S> long_options,
           S value_name, S& dest, F func) {
   auto f = [=, &dest](const S& str) {
      dest = str;
      func();
   };
   return Param<decltype(f)>(std::move(short_options), std::move(long_options), value_name, std::move(f))
      .extra(ct::Cell() << "Default value: " << color::fg_cyan << dest << color::reset);
}

} // be::cli

#endif
