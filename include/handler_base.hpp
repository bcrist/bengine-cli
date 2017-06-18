#pragma once
#ifndef BE_CLI_HANDLER_BASE_HPP_
#define BE_CLI_HANDLER_BASE_HPP_

#include <be/core/be.hpp>
#include <be/ctable/table.hpp>
#include <be/core/id.hpp>
#include <cassert>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class HandlerBase {
public:
   void describe(Id, ct::Table&, bool, const S&) const { }

   std::size_t raw_position_count() const {
      return std::end(Derived::raw_positions) - std::begin(Derived::raw_positions);
   }
   I32 raw_position(std::size_t index) const {
      assert(raw_position_count() > index);
      return *(std::begin(Derived::raw_positions) + index);
   }

   std::size_t short_option_count() const {
      return std::end(Derived::short_options) - std::begin(Derived::short_options);
   }
   S short_option(std::size_t index) const {
      assert(short_option_count() > index);
      return *(std::begin(Derived::short_options) + index);
   }

   std::size_t long_option_count() const {
      return std::end(Derived::long_options) - std::begin(Derived::long_options);
   }
   S long_option(std::size_t index) const {
      assert(long_option_count() > index);
      return *(std::begin(Derived::long_options) + index);
   }

   bool generic_option_handler() const {
      return Derived::generic_options::value;
   }

   std::size_t position_count() const {
      return std::end(Derived::positions) - std::begin(Derived::positions);
   }
   I32 position(std::size_t index) const {
      assert(position_count() > index);
      return *(std::begin(Derived::positions) + index);
   }

   // void operator()(HandlerContext& ctx);

protected:
   static const std::array<I32, 0> raw_positions;
   static const std::array<S, 0> short_options;
   static const std::array<S, 0> long_options;
   using generic_options = False;
   static const std::array<I32, 0> positions;
};

template <typename Derived>
const std::array<I32, 0> HandlerBase<Derived>::raw_positions;

template <typename Derived>
const std::array<S, 0> HandlerBase<Derived>::short_options;

template <typename Derived>
const std::array<S, 0> HandlerBase<Derived>::long_options;

template <typename Derived>
const std::array<I32, 0> HandlerBase<Derived>::positions;

} // be::cli

#endif

