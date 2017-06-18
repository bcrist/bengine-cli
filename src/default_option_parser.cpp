#include "pch.hpp"
#include "default_option_parser.hpp"

namespace be::cli {

OptionParseResult default_option_parser(const S& arg) {
   OptionParseResult result;
   result.is_long_option = false;

   if (!arg.empty() && arg[0] == '-') {
      result.prefix.append(1, '-');
      if (arg.length() > 1 && arg[1] == '-') {
         // long option
         result.prefix.append(1, '-');
         result.is_long_option = true;
         auto first = arg.begin() + 2;
         for (auto it = first, end = arg.end(); it != end; ++it) {
            char c = *it;
            if (c == '=' || c == ':') {
               result.options.push_back(S(first, it));
               result.values.push_back(S(it + 1, end));
               break;
            }
         }
         if (result.options.empty()) {
            // didn't find value; use whole arg as option
            result.options.push_back(S(first, arg.end()));
         }
      } else {
         // short option
         for (auto it = arg.begin() + 1, end = arg.end(); it != end; ++it) {
            char c = *it;
            if (c == '=' || c == ':') {
               result.values.push_back(S(it + 1, end));
               break;
            } else {
               result.options.push_back(S(1, c));
            }
         }
      }
   } // else not an option

   return result;
}

} // be::cli

