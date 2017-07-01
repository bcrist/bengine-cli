#include "pch.hpp"
#include "option_error.hpp"

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
OptionError::OptionError(const HandlerContext& context, const S& msg)
   : ArgumentError(context, msg),
     option_(context.option()) { }

///////////////////////////////////////////////////////////////////////////////
OptionError::OptionError(const HandlerContext& context, const char* msg)
   : ArgumentError(context, msg),
     option_(context.option()) { }

///////////////////////////////////////////////////////////////////////////////
OptionError::OptionError(I32 raw_position, S argument, S option, const S& msg)
   : ArgumentError(raw_position, argument, msg),
     option_(option) { }

///////////////////////////////////////////////////////////////////////////////
OptionError::OptionError(I32 raw_position, S argument, S option, const char* msg)
   : ArgumentError(raw_position, argument, msg),
     option_(option) { }

///////////////////////////////////////////////////////////////////////////////
OptionError::OptionError(OptionError&& other) noexcept
   : ArgumentError(std::move(other)),
     option_(std::move(other.option_)) { }

///////////////////////////////////////////////////////////////////////////////
OptionError& OptionError::operator=(OptionError&& other) noexcept {
   *((ArgumentError*)this) = *(ArgumentError*)&other;
   option_ = std::move(other.option_);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
const S& OptionError::option() const {
   return option_;
}

} // be::cli
