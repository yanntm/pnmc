#pragma once

#include <functional> // hash
#include <ostream>

#include <sdd/util/hash.hh>
#include <sdd/values/flat_set.hh>

#include "shared/pn/transition.hh"

namespace pnmc { namespace mc { namespace classic {

/*------------------------------------------------------------------------------------------------*/

struct pre_clock
{
  const unsigned int lower_clock;

  sdd::values::flat_set<unsigned int>
  operator()(const sdd::values::flat_set<unsigned int>& val)
  const
  {
    sdd::values::values_traits<sdd::values::flat_set<unsigned int>>::builder builder;
    builder.reserve(val.size());
    for (auto cit = val.lower_bound(lower_clock); cit != val.cend(); ++cit)
    {
      if (*cit != pn::sharp)
      {
        builder.insert(builder.end(), *cit);
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
  operator==(const pre_clock& lhs, const pre_clock& rhs)
  noexcept
  {
    return lhs.lower_clock == rhs.lower_clock;
  }

  friend
  std::ostream&
  operator<<(std::ostream& os, const pre_clock& p)
  {
    return os << "pre_clock(" << p.lower_clock << ")";
  }
};

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::classic

namespace std
{

/*------------------------------------------------------------------------------------------------*/

template <>
struct hash<pnmc::mc::classic::pre_clock>
{
  std::size_t
  operator()(const pnmc::mc::classic::pre_clock& p)
  const noexcept
  {
    using namespace sdd::hash;
    return seed(3567850027) (val(p.lower_clock));
  }
};

/*------------------------------------------------------------------------------------------------*/

} // namespace std
