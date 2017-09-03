#pragma once
#ifndef BE_CLI_VERBOSITY_PARAM_HPP_
#define BE_CLI_VERBOSITY_PARAM_HPP_

#include "option_handler_base.hpp"
#include <be/core/exceptions.hpp>
#include <be/core/logging.hpp>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
class VerbosityParam final : public OptionHandlerBase<VerbosityParam> {
public:
   VerbosityParam(std::initializer_list<S> short_options, std::initializer_list<S> long_options, S value_name, U16* verbosity_mask)
      : OptionHandlerBase<VerbosityParam>(std::move(short_options), std::move(long_options)),
        value_name_(std::move(value_name)),
        verbosity_mask_(verbosity_mask) { }

   S option_value_name_() const {
      return value_name_;
   }

   virtual void handle(HandlerContext& ctx) override;

private:
   S value_name_;
   U16* verbosity_mask_;
};

///////////////////////////////////////////////////////////////////////////////
VerbosityParam verbosity_param(std::initializer_list<S> short_options,
                               std::initializer_list<S> long_options,
                               S value_name, U16& verbosity_mask);

} // be::cli

#endif
