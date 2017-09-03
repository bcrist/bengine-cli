#pragma once
#ifndef BE_CLI_HANDLER_HPP_
#define BE_CLI_HANDLER_HPP_

#include "handler_context.hpp"
#include <be/core/id.hpp>
#include <be/ctable/table.hpp>

namespace be::cli {

class Processor;

///////////////////////////////////////////////////////////////////////////////
class Handler {
public:
   virtual ~Handler() = default;

   virtual std::size_t raw_position_count() const noexcept = 0;
   virtual I32 raw_position(std::size_t n) const noexcept = 0;

   virtual std::size_t position_count() const noexcept = 0;
   virtual I32 position(std::size_t n) const noexcept = 0;

   virtual std::size_t short_option_count() const noexcept = 0;
   virtual S short_option(std::size_t n) const noexcept = 0;
   
   virtual std::size_t long_option_count() const noexcept = 0;
   virtual S long_option(std::size_t n) const noexcept = 0;
   
   virtual bool handles_generic_options() const noexcept = 0;
   
   virtual void describe(Id section, ct::Table& table, bool verbose, const S& query) const = 0;
   
   virtual void handle(HandlerContext& ctx) = 0;
};

} // be::cli

#endif
