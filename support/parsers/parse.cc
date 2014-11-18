#include <iostream>
#include <istream>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "support/parsers/ndr.hh"
#include "support/parsers/net.hh"
#include "support/parsers/nupn.hh"
#include "support/parsers/parse.hh"
#include "support/parsers/pnml.hh"

namespace pnmc { namespace parsers {
  
/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
parse(const configuration& conf)
{
  boost::filesystem::ifstream file_stream;
  std::istream* in = conf.file
                   ? file_stream.open(*conf.file), &file_stream
                   : &std::cin;

  if (in->peek() == std::char_traits<char>::eof())
  {
    throw std::runtime_error("Empty input");
  }

  switch (conf.file_type)
  {
    case (conf::pn_format::ndr)  : return parsers::ndr(*in);
    case (conf::pn_format::net)  : return parsers::net(*in);
    case (conf::pn_format::nupn) : return parsers::nupn(*in);
    case (conf::pn_format::pnml) : return parsers::pnml(*in);
  }
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
