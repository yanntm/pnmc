#include <iostream>
#include <istream>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#endif
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "shared/parsers/nupn.hh"
#include "shared/parsers/parse.hh"
#include "shared/parsers/pnml.hh"
#include "shared/parsers/tina.hh"
#include "shared/parsers/xml.hh"
#include "shared/util/paths.hh"

namespace pnmc { namespace parsers {
  
/*------------------------------------------------------------------------------------------------*/

std::shared_ptr<pn::net>
parse(const configuration& conf)
{
  std::istream* in;
  boost::filesystem::ifstream file_stream;
  boost::iostreams::filtering_istream decompressor;

  if (conf.file) // don't read stdin
  {
    const auto& file = *conf.file;

    if (conf.decompress)
    {
      file_stream.open(file, std::ios_base::in | std::ios_base::binary);
    }
    else
    {
      file_stream.open(file);
    }
  }

  if (conf.decompress)
  {
    decompressor.push(boost::iostreams::gzip_decompressor());
    if (not conf.file) // read stdin
    {
      decompressor.push(std::cin);
    }
    else
    {
      decompressor.push(file_stream);
    }
    in = &decompressor;
  }
  else
  {
    if (not conf.file) // read stdin
    {
      in = &std::cin;
    }
    else
    {
      in = &file_stream;
    }
  }

  if (in->peek() == std::char_traits<char>::eof())
  {
    throw std::runtime_error("Empty file or can't read from compressed file");
  }

  switch (conf.file_type)
  {
    case (conf::pn_format::nupn) : return parsers::nupn(*in);
    case (conf::pn_format::pnml) : return parsers::pnml(*in);
    case (conf::pn_format::tina) : return parsers::tina(*in);
    case (conf::pn_format::xml)  : break;
  }
  return parsers::xml(*in);
}

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
