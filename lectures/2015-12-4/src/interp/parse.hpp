#pragma once

#include <list>
#include <string>
#include <cctype>

namespace parse
{

  template <typename T>
  using list = std::list<T>;
  using string = std::string;
  using string_size = std::string::size_type;

  list<string> tokenize(const string&);
  bool isparen(const char&);

}
