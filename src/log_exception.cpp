#include "pch.hpp"
#include "log_exception.hpp"
#include <be/core/logging.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
void log_exception(const OptionError& e, v::Verbosity verbosity, Log& log, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_index) << e.raw_position()
      & attr(ids::log_attr_argument) << S(e.argument())
      & attr(ids::log_attr_option) << S(e.option())
      | log;
}

///////////////////////////////////////////////////////////////////////////////
void log_exception(const ArgumentError& e, v::Verbosity verbosity, Log& log, const char* source) {
   be_log(verbosity, source) << S(e.what())
      & attr(ids::log_attr_index) << e.raw_position()
      & attr(ids::log_attr_argument) << S(e.argument())
      | log;
}

} // be::cli
