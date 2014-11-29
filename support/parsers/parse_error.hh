#pragma once

#include <exception>
#include <string>

namespace pnmc { namespace parsers {

/*------------------------------------------------------------------------------------------------*/

struct parse_error final
  : public std::exception
{
private:

  const std::string message_;

public:

  parse_error(const std::string& message);
  parse_error();

  ~parse_error() noexcept;

  const char* what() const noexcept;
};

/*------------------------------------------------------------------------------------------------*/

struct unsupported_error final
  : public std::exception
{
private:

  const std::string message_;

public:

  unsupported_error(const std::string& message);
  unsupported_error();

  ~unsupported_error() noexcept;

  const char* what() const noexcept;
};


/*------------------------------------------------------------------------------------------------*/

}} // namespace pnmc::parsers
