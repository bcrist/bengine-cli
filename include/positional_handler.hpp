#pragma once
#ifndef BE_CLI_POSITIONAL_HANDLER_HPP_
#define BE_CLI_POSITIONAL_HANDLER_HPP_

#include "option_handler_base.hpp"

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename F>
class PositionalHandler final : public HandlerBase<Param<F>> {
public:
   PositionalHandler(F func = F())
      : func_(std::move(func)) { }

   template <typename... Ts>
   PositionalHandler<F>& position(I32 pos, Ts... args) & {
      position_(pos, args...);
      return *this;
   }

   template <typename... Ts>
   PositionalHandler<F> position(I32 pos, Ts... args) && {
      position_(pos, args...);
      return std::move(*this);
   }

   template <typename... Ts>
   PositionalHandler<F>& raw_position(I32 pos, Ts... args) & {
      raw_position_(pos, args...);
      return *this;
   }

   template <typename... Ts>
   PositionalHandler<F> raw_position(I32 pos, Ts... args) && {
      raw_position_(pos, args...);
      return std::move(*this);
   }

   PositionalHandler<F>& when(std::function<bool()> func) & {
      enabled_ = std::move(func);
      return *this;
   }

   PositionalHandler<F> when(std::function<bool()> func) && {
      enabled_ = std::move(func);
      return std::move(*this);
   }

   virtual std::size_t raw_position_count() const noexcept override {
      return raw_positions_.size();
   }

   virtual I32 raw_position(std::size_t index) const noexcept override {
      assert(raw_position_count() > index);
      return raw_positions_[index];
   }

   virtual std::size_t position_count() const noexcept override {
      return positions_.size();
   }
   virtual I32 position(std::size_t index) const noexcept override {
      assert(position_count() > index);
      return positions_[index];
   }

   virtual void handle(HandlerContext& ctx) override {
      if (enabled_ && !enabled_()) {
         return;
      }

      try {
         if (func_(ctx.argument())) {
            if (ctx.phase() == HandlerContext::positional_phase) {
               ctx.increment_position(true);
            }
            ctx.handled(true);
            ctx.stop_after_phase(true);
         }
      } catch (const std::runtime_error& e) {
         throw ArgumentError(ctx, e.what());
      }
   }

private:
   template <typename... Ts>
   void position_(I32 pos, Ts... args) {
      if (std::find(positions_.begin(), positions_.end(), pos) == positions_.end()) {
         positions_.push_back(pos);
      }
      position_(args...);
   }
   void position_() { }

   template <typename... Ts>
   void raw_position_(I32 pos, Ts... args) {
      if (std::find(raw_positions_.begin(), raw_positions_.end(), pos) == raw_positions_.end()) {
         raw_positions_.push_back(pos);
      }
      raw_position_(args...);
   }
   void raw_position_() { }

   std::vector<I32> raw_positions_;
   std::vector<I32> positions_;
   std::function<bool()> enabled_;
   F func_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename F>
PositionalHandler<F> arg(F func = F()) {
   return PositionalHandler<F>(std::move(func));
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
PositionalHandler<F> any_raw(F func = F()) {
   return PositionalHandler<F>(std::move(func)).raw_position(-1);
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
PositionalHandler<F> any(F func = F()) {
   return PositionalHandler<F>(std::move(func)).position(-1);
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
PositionalHandler<F> nth_raw(I32 n, F func = F()) {
   return PositionalHandler<F>(std::move(func)).raw_position(n);
}

///////////////////////////////////////////////////////////////////////////////
template <typename F>
PositionalHandler<F> nth(I32 n, F func = F()) {
   return PositionalHandler<F>(std::move(func)).position(n);
}

} // be::cli

#endif
