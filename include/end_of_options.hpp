#pragma once
#ifndef BE_CLI_END_OF_OPTIONS_HPP_
#define BE_CLI_END_OF_OPTIONS_HPP_

#include "option_handler_base.hpp"
#include "processor.hpp"
#include "option_error.hpp"
#include <be/core/logging.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class EndOfOptionsFlag final : public OptionHandlerBase<EndOfOptionsFlag> {
public:   
   EndOfOptionsFlag(std::initializer_list<S> short_options, std::initializer_list<S> long_options)
      : OptionHandlerBase<EndOfOptionsFlag>(std::move(short_options), std::move(long_options)) { }

   void operator()(HandlerContext& ctx) {
      if (enabled_ && !enabled_()) {
         return;
      }

      if (ctx.option_value_count() > 0) {
         if (throw_on_error_) {
            throw OptionError(ctx, "Option cannot take a value!");
         } else {
            be_short_warn() << "Ignoring '" << S(ctx.option()) << "': cannot take a value!" | default_log();
            ctx.handled(true);
            return;
         }
      }
      ctx.processor().options(false);
      ctx.handled(true);
      ctx.stop_after_phase(true);
   }
};

///////////////////////////////////////////////////////////////////////////////
EndOfOptionsFlag end_of_options(const S& desc, bool require_long_prefix = true) {
   if (require_long_prefix) {
      return EndOfOptionsFlag({ }, { "" }).desc(desc);
   } else {
      return EndOfOptionsFlag({ "" }, { "" }).desc(desc);
   }
}

///////////////////////////////////////////////////////////////////////////////
EndOfOptionsFlag end_of_options(bool require_long_prefix = true) {
   return end_of_options("Stops parsing options.", require_long_prefix)
      .extra(ct::Cell() << nl << "Any arguments that follow will not be treated as options, even if they begin with '-'.");
}

} // be::cli

#endif
