#pragma once

#include <algorithm>  // copy
#include <functional> // hash
#include <ostream>

#include "mc/classic/sdd.hh"
#include "support/pn/types.hh"

namespace pnmc { namespace mc { namespace classic {

/*------------------------------------------------------------------------------------------------*/

/// @brief An inhibitor arc.
struct inhibitor
{
  const pn::valuation_type valuation;

  flat_set
  operator()(const flat_set& val)
  const
  {
    flat_set_builder builder;
    builder.reserve(val.size());
    // Only keep values that are less than the requested valuation.
    std::copy(val.cbegin(), val.lower_bound(valuation), std::inserter(builder, builder.end()));
    return std::move(builder);
  }

  bool
  selector()
  const noexcept
  {
    return true;
  }

  /// @brief Equality of two inhibitor arcs.
  friend
  bool
  operator==(const inhibitor& lhs, const inhibitor& rhs)
  noexcept
  {
    return lhs.valuation == rhs.valuation;
  }

  /// @brief Textual output of an inhibitor arc.
  friend
  std::ostream&
  operator<<(std::ostream& os, const inhibitor& i)
  {
    return os << "inhib(" << i.valuation << ")";
  }
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::classic

namespace std {

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::classic::inhibitor>
{
  std::size_t
  operator()(const pnmc::mc::classic::inhibitor& i)
  const noexcept
  {
    using namespace sdd::hash;
    return seed(9967321975) (val(i.valuation));
  }
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
