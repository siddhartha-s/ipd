#pragma once

#include<cstddef>
#include<vector>
#include<utility>

namespace union_find
{ 
  
  class int_sets {
  
  public:

    // default constructor
    int_sets();
    // construct a representation of n singleton sets:
    // {0}, {1}, ... , {n-1}
    int_sets(size_t);
    // find the set representative of an integer
    int find(int);
    // combine two sets, given members of each
    void do_union(int, int);
    
  private:

  };
    
} // namespace union_find
