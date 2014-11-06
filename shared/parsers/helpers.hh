#pragma once

#include <istream>
#include <string>
#include <vector>

#include "shared/parsers/helpers.hh"

namespace pnmc { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

/// @brief Input stream manipulator to detect a keyword.
struct kw
{
  const std::string k_;
  
  kw(const std::string& k);

  friend
  std::istream&
  operator>>(std::istream& in, const kw& manip);
};

/*------------------------------------------------------------------------------------------------*/

/// @brief Split a range with a delimiter.
std::vector<std::string>
split(std::string::const_iterator cit, std::string::const_iterator cend, char delim);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
