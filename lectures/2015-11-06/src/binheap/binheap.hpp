#pragma once

#include<vector>

namespace binheap
{

  using idx = std::vector<int>::size_type;

  // a binheap is a vector<int>
  // interpretation:
  // heap[0] is the root
  // heap[parent(i)] is the parent of heap[i]
  // heap[left(i)] is the left child of heap[i]
  // heap[right(i)] is the right child of heap[i]
  
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

  idx parent(idx);
  idx left(idx);
  idx right(idx);
  

}
