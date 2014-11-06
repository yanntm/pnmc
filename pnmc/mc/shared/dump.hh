#pragma once

#include <sdd/tools/dot/force_hypergraph.hh>
#include <sdd/tools/dot/homomorphism.hh>
#include <sdd/tools/dot/sdd.hh>
#include <sdd/tools/js.hh>
#include <sdd/tools/sdd_statistics.hh>
#include <sdd/tools/serialization.hh>

#include <boost/filesystem/fstream.hpp>

#include <cereal/archives/json.hpp>

#include "conf/configuration.hh"
#include "mc/shared/results.hh"
#include "mc/shared/results_serialize.hh"
#include "mc/shared/statistics.hh"
#include "mc/shared/statistics_serialize.hh"
#include "shared/pn/net.hh"
#include "shared/pn/statistics.hh"
#include "shared/pn/statistics_serialize.hh"
#include "shared/util/paths.hh"

namespace pnmc { namespace mc { namespace shared {

/*------------------------------------------------------------------------------------------------*/

/// @brief Export SDD to the DOT format when required by the configuration.
template <typename SDD, typename Order>
void
dump_sdd_dot(const conf::configuration& conf, const SDD& s, const Order& o)
{
  if (conf.final_sdd_dot_file)
  {
    boost::filesystem::ofstream file(*conf.final_sdd_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(s, o) << std::endl;
    }
    else
    {
      std::cerr << "Can't export state space to " << *conf.final_sdd_dot_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Export statistics to a JSON file when required by the configuration.
template <typename Manager, typename SDD>
void
dump_json( const conf::configuration& conf, const statistics& stats, const Manager& manager
         , const SDD& s, const pn::net& net)
{
  if (conf.json_file)
  {
    boost::filesystem::ofstream file(*conf.json_file);
    if (file.is_open())
    {
      cereal::JSONOutputArchive archive(file);
      if (not conf.input.file)
      {
        archive(cereal::make_nvp("file", "stdin"));
      }
      else
      {
        archive(cereal::make_nvp("file", conf.input.file->string()));
      }
      archive(cereal::make_nvp("pnmc", stats), cereal::make_nvp("libsdd", manager));
      if (conf.final_sdd_statistics)
      {
        archive(cereal::make_nvp("final sdd", sdd::tools::statistics(s)));
      }
      if (conf.pn_statistics)
      {
        archive(cereal::make_nvp("pn", pn::statistics(net)));
      }
    }
    else
    {
      std::cerr << "Can't export statistics to " << *conf.json_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Export results to a JSON file when required by the configuration.
void
dump_results(const conf::configuration&, const results&);

/*------------------------------------------------------------------------------------------------*/

/// @brief Export the FORCE's hypergraph to the DOT format when required by the configuration.
template <typename Hypergraph>
void
dump_hypergraph_dot(const conf::configuration& conf, const Hypergraph& graph)
{
  if (conf.hypergraph_dot_file)
  {
    boost::filesystem::ofstream file(*conf.hypergraph_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(graph) << std::endl;
    }
    else
    {
      std::cerr << "Can't export FORCE hypergraph to " << *conf.hypergraph_dot_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

/// @brief Export homomorphisms to the DOT format when required by the configuration.
template <typename Homomorphism>
void
dump_hom(const conf::configuration& conf, const Homomorphism& classic, const Homomorphism& sat)
{
  if (conf.hom_json_file)
  {
    boost::filesystem::ofstream file(*conf.hom_json_file);
    if (file.is_open())
    {
      file << sdd::tools::js(classic) << std::endl;
    }
    else
    {
      std::cerr << "Can't export homomorphism to " << *conf.hom_json_file << std::endl;
    }
  }
  if (conf.hom_dot_file)
  {
    boost::filesystem::ofstream file(*conf.hom_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(classic) << std::endl;
    }
    else
    {
      std::cerr << "Can't export homomorphism to " << *conf.hom_dot_file << std::endl;
    }
  }
  if (conf.sat_hom_dot_file)
  {
    boost::filesystem::ofstream file(*conf.sat_hom_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(sat) << std::endl;
    }
    else
    {
      std::cerr << "Can't export homomorphism to " << *conf.sat_hom_dot_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::shared
