#include <fstream>

#include <sdd/tools/dot/force_hypergraph.hh>
#include <sdd/tools/dot/homomorphism.hh>
#include <sdd/tools/dot/sdd.hh>
#include <sdd/tools/sdd_statistics.hh>
#include <sdd/tools/serialization.hh>

#include <boost/filesystem/fstream.hpp>

#include <cereal/archives/json.hpp>

#include "mc/classic/dump.hh"
#include "mc/classic/results_serialize.hh"
#include "mc/classic/statistics_serialize.hh"
#include "pn/statistics.hh"
#include "pn/statistics_serialize.hh"
#include "util/paths.hh"

namespace pnmc { namespace mc { namespace classic {

/*------------------------------------------------------------------------------------------------*/

void
dump_sdd_dot( const conf::configuration& conf, const sdd::SDD<sdd::conf1>& s
            , const sdd::order<sdd::conf1>& o)
{
  if (conf.export_final_sdd_dot_file)
  {
    boost::filesystem::ofstream file(*conf.export_final_sdd_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(s, o) << std::endl;
    }
    else
    {
      std::cerr << "Can't export state space to " << *conf.export_final_sdd_dot_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

void
dump_json( const conf::configuration& conf, const statistics& stats
         , const sdd::manager<sdd::conf1>& manager, const sdd::SDD<sdd::conf1>& s
         , const pn::net& net)
{
  if (conf.json_file)
  {
    boost::filesystem::ofstream file(*conf.json_file);
    if (file.is_open())
    {
      cereal::JSONOutputArchive archive(file);
      if (not conf.read_stdin)
      {
        archive(cereal::make_nvp("file", conf.file_name));
      }
      archive( cereal::make_nvp("pnmc", stats)
             , cereal::make_nvp("libsdd", manager));
      if (conf.final_sdd_statistics)
      {
        archive(cereal::make_nvp("final sdd", sdd::tools::sdd_statistics<sdd::conf1>(s)));
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

void
dump_results(const conf::configuration& conf, const results& res)
{
  if (conf.results_json_file)
  {
    boost::filesystem::ofstream file(*conf.results_json_file);
    if (file.is_open())
    {
      cereal::JSONOutputArchive archive(file);
      archive(cereal::make_nvp("pnmc", res));
    }
    else
    {
      std::cerr << "Can't export results to " << *conf.results_json_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

void
dump_hypergraph_dot( const conf::configuration& conf
                   , const sdd::force::hypergraph<sdd::conf1>& graph)
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

void
dump_hom_dot( const conf::configuration& conf, const sdd::homomorphism<sdd::conf1>& classic
            , const sdd::homomorphism<sdd::conf1>& sat)
{
  if (conf.export_hom_to_dot_file)
  {
    boost::filesystem::ofstream file(*conf.export_hom_to_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(classic) << std::endl;
    }
    else
    {
      std::cerr << "Can't export homomorphism to " << *conf.export_hom_to_dot_file << std::endl;
    }
  }
  if (conf.export_sat_hom_to_dot_file)
  {
    boost::filesystem::ofstream file(*conf.export_sat_hom_to_dot_file);
    if (file.is_open())
    {
      file << sdd::tools::dot(sat) << std::endl;
    }
    else
    {
      std::cerr << "Can't export homomorphism to " << *conf.export_sat_hom_to_dot_file << std::endl;
    }
  }
}

/*------------------------------------------------------------------------------------------------*/

}}} // namespace pnmc::mc::classic

namespace sdd { namespace values {

/*------------------------------------------------------------------------------------------------*/

template <>
struct display_value<unsigned int>
{
  void
  operator()(std::ostream& os, unsigned int v)
  const
  {
    if (v == pnmc::pn::sharp)
    {
      os << "#";
    }
    else
    {
      os << v;
    }
  }
};

/*------------------------------------------------------------------------------------------------*/

}} // namespace sdd::values
