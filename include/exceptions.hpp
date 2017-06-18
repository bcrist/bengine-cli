#pragma once
#ifndef BE_CLI_EXCEPTIONS_HPP_
#define BE_CLI_EXCEPTIONS_HPP_

#include "handler_context.hpp"
#include <be/core/exceptions.hpp>

namespace be::cli {

class ArgumentException : public FatalException {
public:
   template <typename... Ts>
   explicit ArgumentException(const HandlerContext& context, Ts&&... args)
      : FatalException(std::forward<Ts>(args)...),
        raw_position_(context.raw_position()),
        arg_(context.argument()) { }

   template <typename... Ts>
   ArgumentException(I32 raw_position, S argument, Ts&&... args)
      : FatalException(std::forward<Ts>(args)...),
        raw_position_(raw_position),
        arg_(std::move(argument)) { }

   std::size_t raw_position() const {
      return raw_position_;
   }

   const S& argument() const {
      return arg_;
   }

private:
   std::size_t raw_position_;
   S arg_;
};

class OptionException : public ArgumentException {
public:
   template <typename... Ts>
   explicit OptionException(const HandlerContext& context, Ts&&... args)
      : ArgumentException(context, std::forward<Ts>(args)...),
        option_(context.option()) { }

   template <typename... Ts>
   OptionException(I32 raw_position, S argument, S option, Ts&&... args)
      : ArgumentException(raw_position, std::move(argument), std::forward<Ts>(args)...),
        option_(std::move(option)) { }

   const S& option() const {
      return option_;
   }

private:
   S option_;
};

} // be::cli

#endif
