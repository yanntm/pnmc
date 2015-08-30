/// @file
/// @copyright The code is licensed under the BSD License
///            <http://opensource.org/licenses/BSD-2-Clause>,
///            Copyright (c) 2013-2015 Alexandre Hamez.
/// @author Alexandre Hamez

#pragma once

#include <functional> // hash
#include <ostream>

#include <sdd/util/hash.hh>

#include "mc/classic/sdd.hh"
#include "support/pn/types.hh"

namespace pnmc { namespace mc { namespace classic {

/*------------------------------------------------------------------------------------------------*/

struct enabled
{
  const pn::valuation_type pre;
  const pn::valuation_type post;

  flat_set
  operator()(const flat_set& val)
  const
  {
    auto builder = flat_set_builder{};
    builder.reserve(val.size());
    for (const auto v : val)
    {
      if ((v + post) >= pre)
      {
        builder.insert(builder.end(), v);
      }
    }
    return std::move(builder);
  }

  bool
  selector()
  const noexcept
  {
    return true;
  }

  friend
  bool
  operator==(const enabled& lhs, const enabled& rhs)
  noexcept
  {
    return lhs.pre == rhs.pre and lhs.post == rhs.post;
  }

  friend
  std::ostream&
  operator<<(std::ostream& os, const enabled& e)
  {
    return os << "enabled(" << e.pre << "," << e.post << ")";
  }
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::classic

namespace std
{

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::classic::enabled>
{
  std::size_t
  operator()(const pnmc::mc::classic::enabled& e)
  const noexcept
  {
    using namespace sdd::hash;
    return seed(8067541972) (val(e.pre)) (val(e.post));
  }
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
