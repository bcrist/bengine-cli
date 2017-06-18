#pragma once
#ifndef BE_CLI_DOC_HANDLER_BASE_HPP_
#define BE_CLI_DOC_HANDLER_BASE_HPP_

#include "handler_base.hpp"
#include "handler_context.hpp"

namespace be::cli {

///////////////////////////////////////////////////////////////////////////////
template <typename Derived>
class DocHandlerBase : public HandlerBase<Derived> {
public:   
   DocHandlerBase()
      : no_verbose_enabled_(true),
        verbose_enabled_(true),
        no_query_enabled_(true),
        query_enabled_(false) { }

   Derived& enable(bool no_verbose, bool verbose, bool no_query, bool query) & {
      no_verbose_enabled_ = no_verbose;
      verbose_enabled_ = verbose;
      no_query_enabled_ = no_query;
      query_enabled_ = query;
      return *static_cast<Derived*>(this);
   }

   Derived enable(bool no_verbose, bool verbose, bool no_query, bool query) && {
      no_verbose_enabled_ = no_verbose;
      verbose_enabled_ = verbose;
      no_query_enabled_ = no_query;
      query_enabled_ = query;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& verbose(bool verbose = true, bool no_verbose = false) & {
      no_verbose_enabled_ = no_verbose;
      verbose_enabled_ = verbose;
      return *static_cast<Derived*>(this);
   }

   Derived verbose(bool verbose = true, bool no_verbose = false) && {
      no_verbose_enabled_ = no_verbose;
      verbose_enabled_ = verbose;
      return std::move(*static_cast<Derived*>(this));
   }

   Derived& query(bool query = true, bool no_query = true) & {
      no_query_enabled_ = no_query;
      query_enabled_ = query;
      return *static_cast<Derived*>(this);
   }

   Derived query(bool query = true, bool no_query = true) && {
      no_query_enabled_ = no_query;
      query_enabled_ = query;
      return std::move(*static_cast<Derived*>(this));
   }

   void operator()(HandlerContext&) { }

protected:
   bool should_describe_(bool verbose, const S& query) const {
      return !(
         verbose && !verbose_enabled_ ||
         !verbose && !no_verbose_enabled_ ||
         !query.empty() && query_enabled_ ||
         query.empty() && !no_query_enabled_
         );
   }

   bool no_verbose_enabled_;
   bool verbose_enabled_;
   bool no_query_enabled_;
   bool query_enabled_;
};


} // be::cli

#endif
