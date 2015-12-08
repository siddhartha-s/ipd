#pragma once

#include "expressions.hpp"

#include <list>
#include <string>
#include <cctype>
#include <regex>

namespace parse
{

  using namespace expressions;
  using exp = expressions::exp;

  template <typename T>
  using list = std::list<T>;
  using string = std::string;
  using string_size = std::string::size_type;

  list<string> tokenize(const string&);
  bool isparen(const char&);
  bool isNumeric(const string&);

  class Parser 
  {
    
  public:
    
    shared_ptr<exp> parse();
    Parser(const string& str);

  private:
    
    list<string> tokens;
    list<string>::const_iterator cur;
    list<string>::const_iterator end;

    shared_ptr<exp> parse_exp();
    list<shared_ptr<exp>> parse_until_close();
  };

  string run(string exp_string);

}
