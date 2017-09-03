#pragma once
#ifndef BE_CLI_OPTION_HANDLER_BASE_HPP_
#define BE_CLI_OPTION_HANDLER_BASE_HPP_

#include "handler_base.hpp"
#include "handler_context.hpp"
#include "section_ids.hpp"
#include <be/ctable/cell.hpp>
#include <gsl/string_span>
#include <vector>
#include <functional>

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class OptionHandlerBase : public HandlerBase<Derived> {
public:
   OptionHandlerBase(std::initializer_list<S> short_options, std::initializer_list<S> long_options)
      : short_options_(std::move(short_options)),
        long_options_(std::move(long_options)),
        throw_on_error_(true) { }

   Derived& clear_desc() & {
      description_ = ct::Cell();
      return *static_cast<Derived*>(this);
   }

   Derived clear_desc() && {
      description_ = ct::Cell();
      return *static_cast<Derived*>(this);
   }

   Derived& clear_extra() & {
      verbose_extra_ = ct::Cell();
      return *static_cast<Derived*>(this);
   }

   Derived clear_extra() && {
      verbose_extra_ = ct::Cell();
      return *static_cast<Derived*>(this);
   }

   Derived& desc(ct::Cell description) & {
      description_ << description;
      return *static_cast<Derived*>(this);
   }

   Derived desc(ct::Cell description) && {
      description_ << description;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& desc(const S& description) & {
      description_ << description;
      return *static_cast<Derived*>(this);
   }

   Derived desc(const S& description) && {
      description_ << description;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& extra(ct::Cell description) & {
      verbose_extra_ << nl << description;
      return *static_cast<Derived*>(this);
   }

   Derived extra(ct::Cell description) && {
      verbose_extra_ << nl << description;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& extra(const S& description) & {
      verbose_extra_ << nl << description;
      return *static_cast<Derived*>(this);
   }

   Derived extra(const S& description) && {
      verbose_extra_ << nl << description;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& throw_on_error(bool enabled) & {
      throw_on_error_ = enabled;
      return *static_cast<Derived*>(this);
   }

   Derived throw_on_error(bool enabled) && {
      throw_on_error_ = enabled;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& when(std::function<bool()> func) & {
      enabled_ = std::move(func);
      return *static_cast<Derived*>(this);
   }

   Derived when(std::function<bool()> func) && {
      enabled_ = std::move(func);
      return std::move(*static_cast<Derived*>(this));
   }

   virtual void describe(Id section, ct::Table& t, bool verbose, const S& query) const override {
      using namespace ct;

      if (section != ids::cli_describe_section_options_compact &&
          section != ids::cli_describe_section_options_manstyle) {
         return;
      }

      if (description_.empty() && (!verbose || verbose_extra_.empty())) {
         return;
      }

      if (!query.empty()) {
         bool cancel = true;
         if (query[0] == '-') {
            auto q = gsl::cstring_span<>(query).subspan(1);
            if (!q.empty() && q[0] == '-') {
               q = q.subspan(1);
               for (const S& name : long_options_) {
                  if (std::equal(name.begin(), name.end(), q.begin(), q.end())) { // bug in GSL prevents operator== ADL from working correctly here
                     cancel = false;
                     break;
                  }
               }
            } else {
               for (const S& name : short_options_) {
                  if (std::equal(name.begin(), name.end(), q.begin(), q.end())) { // bug in GSL prevents operator== ADL from working correctly here
                     cancel = false;
                     break;
                  }
               }
            }
         } else {
            for (const S& name : short_options_) {
               if (name.find(query) != std::string::npos) {
                  cancel = false;
                  break;
               }
            }
            if (cancel) {
               for (const S& name : long_options_) {
                  if (name.find(query) != std::string::npos) {
                     cancel = false;
                     break;
                  }
               }
            }
         }

         if (cancel) {
            return;
         }
      }

      t << row;
      bool first = true;
      for (const S& name : short_options_) {
         if (first) {
            first = false;
         } else {
            t << color::fg_dark_gray << ", "
               << color::fg_reset;
         }
         t << '-' << name;
      }
      for (const S& name : long_options_) {
         if (first) {
            first = false;
         } else {
            t << color::fg_dark_gray << ", "
               << color::fg_reset;
         }
         t << "--" << name;
      }

      S option_value_name = static_cast<const Derived*>(this)->option_value_name_();
      if (!option_value_name.empty()) {
         bool optional = static_cast<const Derived*>(this)->is_option_value_optional_();

         t << " ";
         if (optional) {
            t << color::fg_dark_gray << "[ ";
         }

         t << color::fg_cyan << option_value_name;

         if (optional) {
            t << color::fg_dark_gray << " ]";
         }

         t << color::fg_reset;
      }

      if (verbose && description_.empty()) {
         if (section == ids::cli_describe_section_options_manstyle) {
            t << row << verbose_extra_;
         } else {
            t << verbose_extra_;
         }
      } else {
         if (section == ids::cli_describe_section_options_manstyle) {
            t << row << description_;
         } else {
            t << description_;
         }

         if (verbose) {
            t.back().back() << verbose_extra_ << nl;
         }
      }
   }

   S option_value_name_() const {
      return S();
   }

   bool is_option_value_optional_() const {
      return false;
   }

   virtual std::size_t short_option_count() const noexcept override {
      return short_options_.size();
   }

   virtual S short_option(std::size_t index) const noexcept override {
      assert(short_option_count() > index);
      return short_options_[index];
   }

   virtual std::size_t long_option_count() const noexcept override {
      return long_options_.size();
   }

   virtual S long_option(std::size_t index) const noexcept override {
      assert(long_option_count() > index);
      return long_options_[index];
   }

protected:
   std::vector<S> short_options_;
   std::vector<S> long_options_;
   ct::Cell description_;
   ct::Cell verbose_extra_;
   std::function<bool()> enabled_;
   bool throw_on_error_;
};

} // be::cli

#endif
