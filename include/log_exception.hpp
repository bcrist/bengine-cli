#pragma once
#ifndef BE_CLI_LOG_EXCEPTION_HPP_
#define BE_CLI_LOG_EXCEPTION_HPP_

#include "option_error.hpp"
#include <be/core/log.hpp>

namespace be::cli {

void log_exception(const OptionError& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);
void log_exception(const ArgumentError& e, Log& log, v::Verbosity verbosity = v::error, const char* source = nullptr);

} // be::cli

#endif
