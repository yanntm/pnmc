#pragma once

#include <vector>

#include "support/pn/net.hh"
#include "support/properties/formulae.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

struct mc_impl
{
  virtual ~mc_impl() {}
  virtual void operator()(const pn::net&, const properties::formulae&) = 0;
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
