#pragma once
#ifndef BE_CLI_DESCRIBE_CONFIG_HPP_
#define BE_CLI_DESCRIBE_CONFIG_HPP_

#include <be/ctable/table_config.hpp>
#include <be/core/id.hpp>
#include <vector>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
struct DescribeSection {
   Id id;
   S heading;
   ct::TableConfig config;
};

using DescribeConfig = std::vector<DescribeSection>;

DescribeConfig default_describe_config(bool man_style_options = false);

} // be::cli

#endif

