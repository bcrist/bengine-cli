#pragma once
#ifndef BE_CLI_ESCAPE_CLI_ARG_HPP_
#define BE_CLI_ESCAPE_CLI_ARG_HPP_

#include <be/core/be.hpp>

namespace be::cli {

S escape_cli_arg(gsl::cstring_span<> arg); // Native implementation

} // be::cli

#endif
