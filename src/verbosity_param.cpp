#include "pch.hpp"
#include "verbosity_param.hpp"
#include "option_error.hpp"
#include <be/util/keyword_parser.hpp>
#include <be/util/parse_numeric_string.hpp>
#include <be/core/logging.hpp>

namespace be::cli {

void VerbosityParam::handle(HandlerContext& ctx) {
   if (enabled_ && !enabled_()) {
      return;
   }

   if (ctx.value_count() == 0 || ctx.is_option(0)) {
      if (throw_on_error_) {
         throw OptionError(ctx, "Option must have a value!");
      } else {
         be_short_warn() << "Ignoring option '" + ctx.option() + "': must specify a value!" | default_log();

         ctx.handled(true);
         ctx.stop_after_phase(true);
         return;
      }
   } else if (ctx.option_value_count() > 1) {
      if (throw_on_error_) {
         throw OptionError(ctx, "Option can't take multiple values!");
      } else {
         be_short_warn() << "Ignoring option '" + ctx.option() + "': cannot take multiple values!" | default_log();

         ctx.handled(true);
         ctx.stop_after_phase(true);
         return;
      }
   } else {
      ctx.handled(true);
      ctx.stop_after_phase(true);
      const S& value = ctx.consume_value_after_argument(0);

      auto parser = std::move(util::KeywordParser<U16>(0)
         (v::nothing, "quiet", "quiet+", "nothing", "none")
         (v::fatal, "fatal")
         (v::error, "error", "errors")
         (v::errors_or_worse, "error+", "errors+", "errors_or_worse")
         (v::warning, "warning", "warnings", "warn")
         (v::warnings_or_worse, "warning+", "warnings+", "warn+", "warnings_or_worse")
         (v::notice, "notice", "notices")
         (v::notices_or_worse, "notice+", "notices+", "notices_or_worse")
         (v::info, "info", "infos")
         (v::info_or_worse, "info+", "infos+", "info_or_worse")
         (v::verbose, "verbose", "verbose_info", "verbose_infos")
         (v::verbose_or_worse, "verbose+", "verbose_info+", "verbose_infos+", "verbose_or_worse")
         (v::everything, "all", "all+", "everything"));

      bool use_new_value = false;
      U16 new_value = 0;
      for (auto begin = value.begin(), end = value.end(), i = begin; begin != end; begin = i) {
         i = std::find(begin, end, '|');
         gsl::cstring_span<> span(&(*begin), i - begin);

         std::error_code ec;
         auto result = parser.parse(span, ec);
         if (ec) {
            ec = std::error_code();
            result = util::parse_numeric_string<U16>(value, ec);
            if (ec) {
               if (throw_on_error_) {
                  throw OptionError(ctx, "Couldn't parse verbosity mask value: " + to_string(span));
               } else {
                  be_short_warn() << "Ignoring unrecognized or out-of-range verbosity mask value: '" + to_string(span) + "'" | default_log();
               }
            }
         }
         if (!ec) {
            new_value |= result;
            use_new_value = true;
         }
         if (i != end) {
            ++i; // skip '|'
         }
      }

      if (use_new_value) {
         *verbosity_mask_ = new_value;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
VerbosityParam verbosity_param(std::initializer_list<S> short_options,
                               std::initializer_list<S> long_options,
                               S value_name, U16& verbosity_mask) {
   using namespace color;
   return VerbosityParam(std::move(short_options), std::move(long_options), std::move(value_name), &verbosity_mask)
      .desc("Sets the logging verbosity mask.")
      .extra(ct::Cell()
         << "Must be an unsigned 16-bit integer (decimal, hex, or octal), or one of these named constants:"
         << fg_cyan << indent << nl
         << "quiet   " << reset << " = " << fg_cyan << "0x0000" << nl
         << "fatal   " << reset << " = " << fg_cyan << "0x0001" << nl
         << "error   " << reset << " = " << fg_cyan << "0x0002" << nl
         << "warning " << reset << " = " << fg_cyan << "0x0004" << nl
         << "notice  " << reset << " = " << fg_cyan << "0x0008" << nl
         << "info    " << reset << " = " << fg_cyan << "0x0010" << nl
         << "verbose " << reset << " = " << fg_cyan << "0x0020" << nl
         << "all     " << reset << " = " << fg_cyan << "0xFFFF" << unindent << nl
         << reset << "Append '" << fg_cyan << '+' << reset
         << "' to any of the preceeding constants to also include everything above it." << nl
         << "Multiple values can be combined using a bitwise OR by delimiting them with '"
         << fg_cyan << '|' << reset << "'." << nl
         << "Default value: " << fg_cyan << "0x" << std::hex << verbosity_mask << reset);
}

} // be::cli
