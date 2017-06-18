#pragma once
#ifndef BE_CLI_HANDLER_HPP_
#define BE_CLI_HANDLER_HPP_

#include "handler_context.hpp"
#include <be/core/id.hpp>
#include <be/ctable/table.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/callable.hpp>
#include <boost/mpl/vector.hpp>

BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_raw_position_count), raw_position_count)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_raw_position), raw_position)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_position_count), position_count)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_position), position)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_short_option_count), short_option_count)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_short_option), short_option)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_long_option_count), long_option_count)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_long_option), long_option)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_generic_option_handler), generic_option_handler)
BOOST_TYPE_ERASURE_MEMBER((be)(cli)(has_describe), describe)

namespace be::cli {

class Processor;

///////////////////////////////////////////////////////////////////////////////
using Handler = boost::type_erasure::any<boost::mpl::vector<
   boost::type_erasure::copy_constructible<>,
   has_raw_position_count<std::size_t(), const boost::type_erasure::_self>,
   has_raw_position<I32(std::size_t), const boost::type_erasure::_self>,
   has_position_count<std::size_t(), const boost::type_erasure::_self>,
   has_position<I32(std::size_t), const boost::type_erasure::_self>,
   has_short_option_count<std::size_t(), const boost::type_erasure::_self>,
   has_short_option<S(std::size_t), const boost::type_erasure::_self>,
   has_long_option_count<std::size_t(), const boost::type_erasure::_self>,
   has_long_option<S(std::size_t), const boost::type_erasure::_self>,
   has_generic_option_handler<bool(), const boost::type_erasure::_self>,
   has_describe<void(Id, ct::Table&, bool, const S&), const boost::type_erasure::_self>,
   boost::type_erasure::callable<void(HandlerContext&)>,
   boost::type_erasure::relaxed>>;

} // be::cli

#endif
