#pragma once

#include <memory> // shared_ptr
#include <string>
#include <vector>

#include <boost/variant.hpp>

#include "shared/pn/place.hh"

namespace pnmc { namespace pn {

/*------------------------------------------------------------------------------------------------*/

// Forward declaration.
struct module_node;

/// @brief
using module = std::shared_ptr<boost::variant<module_node, const place*>>;

/// @brief A recursive module.
struct module_node
{
  /// @brief
  std::vector<module> nested;

  /// @brief
  const std::string id;

  /// @brief Constructor.
  module_node(const std::string&);

  /// @brief
  void
  add_module(const module&);
};

/*------------------------------------------------------------------------------------------------*/

module
make_module(const place&);

module
make_module(const module_node&);

/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::pn
