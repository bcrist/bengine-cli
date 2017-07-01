#pragma once
#ifndef BE_CLI_FLAG_HPP_
#define BE_CLI_FLAG_HPP_

#include "option_handler_base.hpp"
#include "option_error.hpp"
#include <be/core/logging.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename F>
class Flag final : public OptionHandlerBase<Flag<F>> {
public:   
   Flag(std::initializer_list<S> short_options, std::initializer_list<S> long_options, F func = F())
      : OptionHandlerBase<Flag<F>>(std::move(short_options), std::move(long_options)),
        skip_if_has_value_(false),
        ignore_values_(false),
        func_(std::move(func))
   { }

   Flag<F>& skip_if_has_value(bool enabled) & {
      skip_if_has_value_ = enabled;
      return *this;
   }

   Flag<F> skip_if_has_value(bool enabled) && {
      skip_if_has_value_ = enabled;
      return std::move(*this);
   }

   Flag<F>& ignore_values(bool enabled) & {
      ignore_values_ = enabled;
      return *this;
   }

   Flag<F> ignore_values(bool enabled) && {
      ignore_values_ = enabled;
      return std::move(*this);
   }

   void operator()(HandlerContext& ctx) {
      if (ctx.option_value_count() > 0) {
         if (skip_if_has_value_) {
            return;
         }

         if (!ignore_values_) {
            if (throw_on_error_) {
               throw OptionError(ctx, "Option cannot take a value!");
            } else {
               be_short_warn() << "Ignoring option '" << S(ctx.option()) << "': cannot take a value!" | default_log();
               ctx.handled(true);
               return;
            }
         }
      }
      ctx.handled(true);
      try {
         func_();
      } catch (const std::runtime_error& e) {
         throw OptionError(ctx, e.what());
      }
      ctx.stop_after_phase(true);
   }

private:
   bool skip_if_has_value_;
   bool ignore_values_;
   F func_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename F>
Flag<F> flag(std::initializer_list<S> short_options,
             std::initializer_list<S> long_options,
             F func = F()) {
   return Flag<F>(std::move(short_options), std::move(long_options), std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
inline auto flag(std::initializer_list<S> short_options,
                 std::initializer_list<S> long_options,
                 bool& ref) {
   auto func = [&ref](){ ref = true; };
   return Flag<decltype(func)>(std::move(short_options), std::move(long_options), std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
auto flag(std::initializer_list<S> short_options,
          std::initializer_list<S> long_options,
          T& ref, T val) {
   auto func = [&ref,val{std::move(val)}](){ ref = val; };
   return Flag<decltype(func)>(std::move(short_options), std::move(long_options), std::move(func));
}

} // be::cli

#endif
