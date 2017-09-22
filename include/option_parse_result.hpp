#pragma once
#ifndef BE_CLI_OPTION_PARSE_RESULT_HPP_
#define BE_CLI_OPTION_PARSE_RESULT_HPP_

#include <be/core/be.hpp>
#include <vector>

namespace be::cli {

struct OptionParseResult {
   S prefix, suffix;
   bool is_long_option;
   std::vector<S> options;
   std::vector<S> values;
};

} // be::cli

#endif

