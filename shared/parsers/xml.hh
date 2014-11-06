#pragma once

#include <iosfwd>
#include <memory>

#include "shared/pn/net.hh"

namespace pnmc { namespace parsers {
  
/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
xml(std::istream&);

/*------------------------------------------------------------------------------------------------*/

}} // pnmc::parsers
