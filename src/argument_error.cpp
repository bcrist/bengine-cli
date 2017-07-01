#include "pch.hpp"
#include "argument_error.hpp"

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
ArgumentError::ArgumentError(const HandlerContext& context, const S& msg)
   : std::runtime_error(msg),
     raw_position_(context.raw_position()),
     arg_(context.argument()) { }

///////////////////////////////////////////////////////////////////////////////
ArgumentError::ArgumentError(const HandlerContext& context, const char* msg)
   : std::runtime_error(msg),
     raw_position_(context.raw_position()),
     arg_(context.argument()) { }

///////////////////////////////////////////////////////////////////////////////
ArgumentError::ArgumentError(I32 raw_position, S argument, const S& msg)
   : std::runtime_error(msg),
     raw_position_(raw_position),
     arg_(argument) { }

///////////////////////////////////////////////////////////////////////////////
ArgumentError::ArgumentError(I32 raw_position, S argument, const char* msg)
   : std::runtime_error(msg),
     raw_position_(raw_position),
     arg_(argument) { }

///////////////////////////////////////////////////////////////////////////////
ArgumentError::ArgumentError(ArgumentError&& other) noexcept
   : std::runtime_error(other.what()),
     raw_position_(other.raw_position_),
     arg_(other.arg_) { }

///////////////////////////////////////////////////////////////////////////////
ArgumentError& ArgumentError::operator=(ArgumentError&& other) noexcept {
   *((std::runtime_error*)this) = *(std::runtime_error*)&other;
   raw_position_ = other.raw_position_;
   arg_ = std::move(other.arg_);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
std::size_t ArgumentError::raw_position() const {
   return raw_position_;
}

///////////////////////////////////////////////////////////////////////////////
const S& ArgumentError::argument() const {
   return arg_;
}

} // be::cli
