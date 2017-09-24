#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN
#include <be/core/be.hpp>
#include <gsl/string_span>

namespace be::cli {

S escape_cli_arg(SV arg) {
   if (arg.empty()) {
      return "\"\"";
   }

   auto it = std::find_if(arg.begin(), arg.end(), [](char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v'; });
   if (it == arg.end()) {
      return S(arg);
   }

   S val;
   val.reserve(arg.length() + 2);
   val.push_back('\"');

   for (auto it2 = arg.begin(); ; ++it) {
      U32 n_backslashes = 0;

      while (it != arg.end () && *it == '\\') {
         ++it;
         ++n_backslashes;
      }

      if (it == arg.end ()) {
         val.append(n_backslashes * 2, '\\');
         break;
      } else if (*it == '\"') {
         val.append(n_backslashes * 2 + 1, '\\');
         val.push_back (*it);
      } else {
         val.append(n_backslashes, '\\');
         val.push_back (*it);
      }
   }

   val.push_back('\"');

   return val;
}

} // be::cli

#endif
