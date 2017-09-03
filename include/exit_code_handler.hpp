#pragma once
#ifndef BE_CLI_EXIT_CODE_HANDLER_HPP_
#define BE_CLI_EXIT_CODE_HANDLER_HPP_

#include "doc_handler_base.hpp"
#include "section_ids.hpp"
#include <be/ctable/cell.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class ExitCodeHandler final : public DocHandlerBase<ExitCodeHandler> {
public:   
   ExitCodeHandler(int exit_code, ct::Cell description)
      : exit_code_(exit_code),
        description_(std::move(description)) {
      no_verbose_enabled_ = false;
   }

   virtual void describe(Id section, ct::Table& t, bool verbose, const S& query) const override {
      if (section == ids::cli_describe_section_exitcodes && should_describe_(verbose, query)) {
         t << ct::row << exit_code_ << ct::cell << description_;
      }
   }

protected:
   int exit_code_;
   ct::Cell description_;
};


///////////////////////////////////////////////////////////////////////////////
inline ExitCodeHandler exit_code(int code, ct::Cell description) {
   return ExitCodeHandler(code, std::move(description));
}

///////////////////////////////////////////////////////////////////////////////
inline ExitCodeHandler exit_code(int code, const S& description) {
   return ExitCodeHandler(code, ct::Cell() << description);
}

} // be::cli

#endif
