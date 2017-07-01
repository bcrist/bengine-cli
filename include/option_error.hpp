#pragma once
#ifndef BE_CLI_OPTION_ERROR_HPP_
#define BE_CLI_OPTION_ERROR_HPP_

#include "argument_error.hpp"

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class OptionError : public ArgumentError {
public:
   OptionError(const HandlerContext& context, const S& msg);
   OptionError(const HandlerContext& context, const char* msg);
   OptionError(I32 raw_position, S argument, S option, const S& msg);
   OptionError(I32 raw_position, S argument, S option, const char* msg);

   OptionError(const OptionError&) = default;
   OptionError(OptionError&& other) noexcept;
   OptionError& operator=(const OptionError&) = default;
   OptionError& operator=(OptionError&& other) noexcept;

   const S& option() const;

private:
   S option_;
};

} // be::cli

#endif
