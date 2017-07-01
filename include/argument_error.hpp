#pragma once
#ifndef BE_CLI_ARGUMENT_ERROR_HPP_
#define BE_CLI_ARGUMENT_ERROR_HPP_

#include "handler_context.hpp"
#include <be/core/stack_trace_exception.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class ArgumentError : public std::runtime_error {
public:
   ArgumentError(const HandlerContext& context, const S& msg);
   ArgumentError(const HandlerContext& context, const char* msg);
   ArgumentError(I32 raw_position, S argument, const S& msg);
   ArgumentError(I32 raw_position, S argument, const char* msg);

   ArgumentError(const ArgumentError&) = default;
   ArgumentError(ArgumentError&& other) noexcept;
   ArgumentError& operator=(const ArgumentError&) = default;
   ArgumentError& operator=(ArgumentError&& other) noexcept;

   std::size_t raw_position() const;
   const S& argument() const;

private:
   std::size_t raw_position_;
   S arg_;
};

using ArgumentTrace = StackTraceException<ArgumentError>;

} // be::cli

#endif
