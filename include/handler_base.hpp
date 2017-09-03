#pragma once
#ifndef BE_CLI_HANDLER_BASE_HPP_
#define BE_CLI_HANDLER_BASE_HPP_

#include "handler.hpp"
#include <cassert>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class HandlerBase : public Handler {
public:
   virtual void describe(Id, ct::Table&, bool, const S&) const override { }

   virtual std::size_t raw_position_count() const noexcept override {
      return std::end(Derived::raw_positions) - std::begin(Derived::raw_positions);
   }

   virtual I32 raw_position(std::size_t index) const noexcept override {
      assert(raw_position_count() > index);
      return *(std::begin(Derived::raw_positions) + index);
   }

   virtual std::size_t short_option_count() const noexcept override {
      return std::end(Derived::short_options) - std::begin(Derived::short_options);
   }

   virtual S short_option(std::size_t index) const noexcept override {
      assert(short_option_count() > index);
      return *(std::begin(Derived::short_options) + index);
   }

   virtual std::size_t long_option_count() const noexcept override {
      return std::end(Derived::long_options) - std::begin(Derived::long_options);
   }

   virtual S long_option(std::size_t index) const noexcept override {
      assert(long_option_count() > index);
      return *(std::begin(Derived::long_options) + index);
   }

   virtual bool handles_generic_options() const noexcept override {
      return Derived::generic_options::value;
   }

   virtual std::size_t position_count() const noexcept override {
      return std::end(Derived::positions) - std::begin(Derived::positions);
   }

   virtual I32 position(std::size_t index) const noexcept override {
      assert(position_count() > index);
      return *(std::begin(Derived::positions) + index);
   }

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

