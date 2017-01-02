#pragma once
#ifndef BE_CLI_SYNOPSIS_HANDLER_HPP_
#define BE_CLI_SYNOPSIS_HANDLER_HPP_

#include "doc_handler.hpp"
#include "app_name.hpp"

namespace be {
namespace cli {

///////////////////////////////////////////////////////////////////////////////
auto synopsis(S command, ct::Cell cell) {
   auto func = [command{std::move(command)},cell{std::move(cell)}](ct::Table& t){
      using namespace color;
      t << ct::row << command << fg_gray << ' ' << cell;
   };
   return DocHandler<decltype(func)>(ids::cli_describe_section_synopsis, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto synopsis(ct::Cell cell) {
   return synopsis(app_name(), std::move(cell));
}

} // be::cli
} // be

#endif
