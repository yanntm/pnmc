#include "mc/mc.hh"
#include "mc/classic/worker.hh"

namespace pnmc { namespace mc {

/*------------------------------------------------------------------------------------------------*/

mc::mc(const conf::configuration& conf)
  : impl_(std::unique_ptr<mc_impl>(new classic::worker{conf}))
{}

/*------------------------------------------------------------------------------------------------*/

void
mc::operator()(const pn::net& net, const properties::formulae& formulae)
{
  impl_->operator()(net, formulae);
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::mc
