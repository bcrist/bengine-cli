#pragma once
#ifndef BE_CLI_ENUM_PARAM_HPP_
#define BE_CLI_ENUM_PARAM_HPP_

#include "param.hpp"
#include <be/util/keyword_parser.hpp>

namespace be::cli {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::KeywordParser<T> create_enum_parser() {
   util::KeywordParser<T> parser;
   auto values = EnumTraits<T>::values<>();
   for (T value : values) {
      parser(value, EnumTraits<T>::name(value));
   }
   return parser;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
const util::KeywordParser<T>& enum_parser() {
   static util::KeywordParser<T> parser = create_enum_parser<T>();
   return parser;
}

} // be::cli::detail

///////////////////////////////////////////////////////////////////////////////
template <typename T>
auto enum_param(std::initializer_list<S> short_options,
                std::initializer_list<S> long_options,
                S value_name, T& dest) {
   auto func = [=, &dest](const S& str) {
      auto& parser = detail::enum_parser<T>();

      auto result = parser.parse(str);
      if (result.second != util::ParseStringError::none) {
         auto numeric_result = util::parse_numeric_string<typename EnumTraits<T>::underlying_type>(str);
         result.first = static_cast<T>(numeric_result.first);
         result.second = numeric_result.second;
      }

      if (result.second != util::ParseStringError::none) {
         if (result.second == util::ParseStringError::syntax_error) {
            throw Recoverable<>("Unrecognized enum value!");
         } else {
            util::throw_on_error(result);
         }
      }

      if (!EnumTraits<T>::is_valid(result.first)) {
         throw Recoverable<>("Invalid enum value!");
      } else {
         dest = result.first;
      }
   };

   ct::Cell extra;
   extra << "Valid values: ";

   bool first = true;
   auto values = EnumTraits<T>::values<>();
   for (T value : values) {
      if (first) {
         first = false;
      } else {
         extra << ", ";
      }
      extra << color::fg_cyan << EnumTraits<T>::name(value) << color::reset;
   }

   extra << "." << nl << "Default value: "
         << color::fg_cyan << EnumTraits<T>::name(dest) << color::reset;

   return Param<decltype(func)>(std::move(short_options), std::move(long_options), value_name, std::move(func))
      .extra(std::move(extra));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename F>
auto enum_param(std::initializer_list<S> short_options,
                std::initializer_list<S> long_options,
                S value_name, T& dest, F func = F()) {
   auto f = [=, &dest](const S& str) {
      auto& parser = detail::enum_parser<T>();

      auto result = parser.parse(str);
      if (result.second != util::ParseStringError::none) {
         auto numeric_result = util::parse_numeric_string<typename EnumTraits<T>::underlying_type>(str);
         result.first = static_cast<T>(numeric_result.first);
         result.second = numeric_result.second;
      }

      if (result.second != util::ParseStringError::none) {
         if (result.second == util::ParseStringError::syntax_error) {
            throw Recoverable<>("Unrecognized enum value!");
         } else {
            util::throw_on_error(result);
         }
      }

      if (!EnumTraits<T>::is_valid(result.first)) {
         throw Recoverable<>("Invalid enum value!");
      } else {
         dest = result.first;
         func();
      }
   };

   ct::Cell extra;
   extra << "Valid values: ";

   bool first = true;
   auto values = EnumTraits<T>::values<>();
   for (T value : values) {
      if (first) {
         first = false;
      } else {
         extra << ", ";
      }
      extra << color::fg_cyan << EnumTraits<T>::name(value) << color::reset;
   }

   extra << "." << nl << "Default value: "
      << color::fg_cyan << EnumTraits<T>::name(dest) << color::reset;

   return Param<decltype(f)>(std::move(short_options), std::move(long_options), value_name, std::move(f))
      .extra(std::move(extra));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename F>
auto enum_param(std::initializer_list<S> short_options,
                std::initializer_list<S> long_options,
                S value_name, F func = F()) {
   auto f = [=](const S& str) {
      auto& parser = detail::enum_parser<T>();

      auto result = parser.parse(str);
      if (result.second != util::ParseStringError::none) {
         auto numeric_result = util::parse_numeric_string<typename EnumTraits<T>::underlying_type>(str);
         result.first = static_cast<T>(numeric_result.first);
         result.second = numeric_result.second;
      }

      if (result.second != util::ParseStringError::none) {
         if (result.second == util::ParseStringError::syntax_error) {
            throw Recoverable<>("Unrecognized enum value!");
         } else {
            util::throw_on_error(result);
         }
      }

      if (!EnumTraits<T>::is_valid(result.first)) {
         throw Recoverable<>("Invalid enum value!");
      } else {
         func(result.first);
      }
   };

   ct::Cell extra;
   extra << "Valid values: ";

   bool first = true;
   auto values = EnumTraits<T>::values<>();
   for (T value : values) {
      if (first) {
         first = false;
      } else {
         extra << ", ";
      }
      extra << color::fg_cyan << EnumTraits<T>::name(value) << color::reset;
   }

   return Param<decltype(f)>(std::move(short_options), std::move(long_options), value_name, std::move(f))
      .extra(std::move(extra));
}

} // be::cli

#endif
