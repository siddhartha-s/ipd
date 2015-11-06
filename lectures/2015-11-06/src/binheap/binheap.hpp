#pragma once

#include<vector>

namespace binheap
{

  using idx = std::vector<int>::size_type;

  class binheap {
  
  public:
    
    bool isEmpty();
    int getMin();
    void removeMin();
    void add(int);
    
  private:

    std::vector<int> heap;
    void bubbleUp(idx);
    void bubbleDown(idx);
    void swap(idx, idx);
    idx size();

  };

  //bool operator==(binheap, binheap);

  idx parent(idx);
  idx left(idx);
  idx right(idx);
  

}
