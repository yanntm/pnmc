#include <algorithm> // random_shuffle, transform
#include <cassert>
#include <deque>
#include <chrono>
#include <random>
#include <utility>  // pair

#include <sdd/order/order.hh>
#include <sdd/order/strategies/force.hh>

#include "mc/classic/dump.hh"
#include "mc/classic/make_order.hh"

namespace pnmc { namespace mc { namespace classic {

using sdd_conf = sdd::conf1;

/*------------------------------------------------------------------------------------------------*/

struct mk_order_visitor
  : public boost::static_visitor<std::pair<std::string, sdd::order_builder<sdd_conf>>>
{
  using order_identifier = sdd::order_identifier<sdd_conf>;
  using result_type = std::pair<order_identifier, sdd::order_builder<sdd_conf>>;
  using order_builder = sdd::order_builder<sdd_conf>;

  const conf::configuration& conf;
  mutable unsigned int artificial_id_counter;

  mk_order_visitor(const conf::configuration& c)
    : conf(c), artificial_id_counter(0)
  {}

  // Place: base case of the recursion, there's no more possible nested hierarchies.
  result_type
  operator()(const pn::place* p)
  const noexcept
  {
    return std::make_pair(order_identifier(p->id), order_builder());
  }

  // Hierarchy.
  result_type
  operator()(const pn::module_node& m)
  const noexcept
  {
    assert(not m.nested.empty());

    std::deque<result_type> tmp;

    for (const auto& h : m.nested)
    {
      const auto res = boost::apply_visitor(*this, *h);
      tmp.push_back(res);
    }

    std::size_t height = 0;
    for (const auto& p : tmp)
    {
      if (not p.second.empty())
      {
        height += p.second.height();
      }
      else
      {
        height += 1; // place
      }
    }

    order_builder ob;
    if (height <= conf.order_min_height)
    {
      order_identifier id;
      for (const auto& p : tmp)
      {
        if (not p.second.empty())
        {
          ob = p.second << ob;
        }
        else // place
        {
          ob.push(p.first, p.second);
        }
      }
      return result_type(id, ob);
    }
    else
    {
      for (const auto& p : tmp)
      {
        ob.push(p.first, p.second);
      }
    }

    return std::make_pair(order_identifier(m.id) , ob);
  }
};

/*------------------------------------------------------------------------------------------------*/

sdd::order<sdd_conf>
make_order(const conf::configuration& conf, statistics& stats, const pn::net& net)
{
  if (conf.order_ordering_force)
  {
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    using identifier_type = sdd_conf::Identifier;
    using vertex = sdd::force::vertex<identifier_type>;
    using hyperedge = sdd::force::hyperedge<identifier_type>;

    // Temporary placeholder for identifiers.
    std::vector<identifier_type> identifiers;

    // Collect identifiers.
    identifiers.reserve(net.places().size());
    for (const auto& place : net.places())
    {
      if (place.connected())
      {
        identifiers.emplace_back(place.id);
      }
    }

    // The hypergraph that stores connections between the places of the Petri net.
    sdd::force::hypergraph<sdd_conf> graph(identifiers.cbegin(), identifiers.cend());

    // This container will be used again.
    identifiers.clear();

    // A new connection is created for each transition of the Petri net.
    for (const auto& transition : net.transitions())
    {
      for (const auto& arc : transition.pre)
      {
        identifiers.emplace_back(arc.first);
      }

      for (const auto& arc : transition.post)
      {
        identifiers.emplace_back(arc.first);
      }
      graph.add_hyperedge(identifiers.cbegin(), identifiers.cend());

      // We use this container again in the next loop.
      identifiers.clear();
    }
    // Apply the FORCE ordering strategy.
    auto force = sdd::force::worker<sdd_conf>(graph);
    const auto o = force(conf.order_force_iterations);
    stats.force_duration = std::chrono::system_clock::now() - start;
    stats.force_spans = force.spans();

    // Dump the hypergraph to a DOT file if required by the configuration.
    dump_hypergraph_dot(conf, graph);
    return sdd::order<sdd_conf>(o);
  }
  else if (not conf.order_force_flat and net.modules)
  {
    return sdd::order<sdd_conf>(boost::apply_visitor(mk_order_visitor(conf), *net.modules).second);
  }
  else
  {
    sdd::order_builder<sdd_conf> ob;
    if (conf.order_random)
    {
      std::vector<std::string> tmp;
      tmp.reserve(net.places().size());
      for (const auto& place : net.places())
      {
        if (place.connected())
        {
          tmp.emplace_back(place.id);
        }
      }
      std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(tmp.begin(), tmp.end(), g);
      for (const auto& id : tmp)
      {
        ob.push(id);
      }
    }
    else
    {
      for (const auto& place : net.places())
      {
        if (place.connected())
        {
          ob.push(place.id);
        }
      }
    }
    return sdd::order<sdd_conf>(ob);
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::classic
