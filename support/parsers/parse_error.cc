/// @file
/// @copyright The code is licensed under the BSD License
///            <http://opensource.org/licenses/BSD-2-Clause>,
///            Copyright (c) 2013-2015 Alexandre Hamez.
/// @author Alexandre Hamez

#include "parse_error.hh"

namespace pnmc { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

parse_error::parse_error(const std::string& message)
  : message_(message)
{}

parse_error::parse_error()
  : parse_error("No message.")
{}

parse_error::~parse_error()
noexcept
{}

/*------------------------------------------------------------------------------------------------*/

const char*
parse_error::what()
const noexcept
{
  return message_.c_str();
}

/*------------------------------------------------------------------------------------------------*/

unsupported_error::unsupported_error(const std::string& message)
  : message_(message)
{}

unsupported_error::unsupported_error()
  : unsupported_error("No message.")
{}

unsupported_error::~unsupported_error()
noexcept
{}

/*------------------------------------------------------------------------------------------------*/

const char*
unsupported_error::what()
const noexcept
{
  return message_.c_str();
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
