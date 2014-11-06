#pragma once

#include <iosfwd>
#include <memory>

#include "parsers/configuration.hh"
#include "pn/net.hh"

namespace pnmc { namespace parsers {
  
/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
parse(const configuration&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
