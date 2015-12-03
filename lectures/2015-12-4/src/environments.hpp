#pragma once

#include <map>
#include <memory>

#include "expressions.hpp"

namespace expressions
{
  
  class environment
  {

  public:

    value lookup(string);
    void bind(string, shared_ptr<value>);
    
    environment() : parent{nullptr} {}
    environment(shared_ptr<environment> par) : parent{par} {}
    
  private:

    shared_ptr<environment> parent;
    map<string, shared_ptr<value>> bindings;

  }

}
