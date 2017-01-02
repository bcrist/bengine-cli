#pragma once
#ifndef BE_CLI_EXAMPLE_HANDLER_HPP_
#define BE_CLI_EXAMPLE_HANDLER_HPP_

#include "doc_handler.hpp"
#include "app_name.hpp"

namespace be {
namespace cli {

///////////////////////////////////////////////////////////////////////////////
auto example(S command, ct::Cell args, ct::Cell description) {
   auto func = [command{std::move(command)},args{std::move(args)},description{std::move(description)}](ct::Table& t){
      using namespace color;
      t << ct::row << command << fg_gray << ' ';
      t.back().back() << args << reset;
      if (!description.empty()) {
         t << ct::row << description;
      }
   };
   return DocHandler<decltype(func)>(ids::cli_describe_section_examples, std::move(func)).verbose();
}

///////////////////////////////////////////////////////////////////////////////
auto example(ct::Cell args, ct::Cell description) {
   return example(app_name(), std::move(args), std::move(description));
}

///////////////////////////////////////////////////////////////////////////////
auto example(S command, ct::Cell args, S description) {
   auto func = [command{std::move(command)},args{std::move(args)},description{std::move(description)}](ct::Table& t){
      using namespace color;
      t << ct::row << command << fg_gray << ' ';
      t.back().back() << args << reset;
      if (!description.empty()) {
         t << ct::row << description;
      }
   };
   return DocHandler<decltype(func)>(ids::cli_describe_section_examples, std::move(func)).verbose();
}

///////////////////////////////////////////////////////////////////////////////
auto example(ct::Cell args, S description) {
   return example(app_name(), std::move(args), std::move(description));
}

} // be::cli
} // be

#endif
