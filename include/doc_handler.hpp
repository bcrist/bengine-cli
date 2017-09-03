#pragma once
#ifndef BE_CLI_DOC_HANDLER_HPP_
#define BE_CLI_DOC_HANDLER_HPP_

#include "doc_handler_base.hpp"
#include "section_ids.hpp"
#include <be/ctable/cell.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename F>
class DocHandler final : public DocHandlerBase<DocHandler<F>> {
public:
   DocHandler(Id section, F func = F())
      : section_(section),
        func_(std::move(func)) { }

   virtual void describe(Id section, ct::Table& t, bool verbose, const S& query) const override {
      if (section == section_ && this->should_describe_(verbose, query)) {
         func_(t);
      }
   }

protected:
   Id section_;
   F func_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename F>
DocHandler<F> doc(Id section, F func = F()) {
   return DocHandler<F>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto doc(Id section, const char* text) {
   auto func = [text](ct::Table& t){ t << ct::row << text; };
   return DocHandler<decltype(func)>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto doc(Id section, S text) {
   auto func = [text{std::move(text)}](ct::Table& t){ t << ct::row << text; };
   return DocHandler<decltype(func)>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto doc(Id section, ct::Cell cell) {
   auto func = [cell{std::move(cell)}](ct::Table& t){ t << ct::row << cell; };
   return DocHandler<decltype(func)>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto doc(Id section, ct::Row row) {
   auto func = [row{std::move(row)}](ct::Table& t){ t << row; };
   return DocHandler<decltype(func)>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
auto doc(Id section, ct::Table table) {
   auto func = [table{std::move(table)}](ct::Table& t){ t << table; };
   return DocHandler<decltype(func)>(section, std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T> auto prologue(T val = T()) { return doc(ids::cli_describe_section_prologue, std::move(val)); }
template <typename T> auto intro(T val = T()) { return doc(ids::cli_describe_section_intro, std::move(val)); }
template <typename T> auto abstract(T val = T()) { return doc(ids::cli_describe_section_abstract, std::move(val)); }
template <typename T> auto summary(T val = T()) { return doc(ids::cli_describe_section_summary, std::move(val)); }
template <typename T> auto conclusion(T val = T()) { return doc(ids::cli_describe_section_conclusion, std::move(val)); }
template <typename T> auto license(T val = T()) { return doc(ids::cli_describe_section_license, std::move(val)); }
template <typename T> auto epilogue(T val = T()) { return doc(ids::cli_describe_section_epilogue, std::move(val)); }

} // be::cli

#endif
