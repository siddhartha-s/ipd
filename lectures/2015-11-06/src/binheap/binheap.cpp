#include "binheap.hpp"

namespace binheap
{

  bool binheap::isEmpty()
  {
    return heap.empty();
  }

  int binheap::getMin()
  {
    return heap[0];
  }

  idx binheap::size() 
  {
    return heap.size();
  }

  void binheap::add(int newValue)
  {
    heap.push_back(newValue);
    bubbleUp(heap.size() - 1);
  }

  void binheap::removeMin()
  {
    heap[0] = heap[heap.size() - 1];
    heap.pop_back();
    bubbleDown(0);
  }

  void binheap::bubbleUp(idx i) 
  {
    idx par_i = parent(i);
    if (i != 0 && heap[par_i] > heap[i]) {
      swap(par_i, i);
      bubbleUp(par_i);
    }
  }

  void binheap::bubbleDown(idx i) {
    idx left_i = left(i);
    idx right_i = right(i);
    if (right_i < size() && heap[right_i] < heap[i]) {
      if (heap[left_i] < heap[right_i]) {
	swap(left_i, i);
	bubbleDown(left_i); 
      } else {
	swap(right_i, i);
	bubbleDown(right_i); 
      }
    } else if (left_i < size() && heap[left_i] < heap[i]) {
      swap(left_i, i);
      bubbleDown(left_i);
    }
  }

  void binheap::swap(idx i, idx j) {
    int temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
  }
    

  idx parent(idx n)
  {
    return (n - 1) / 2;
  }

  idx left(idx n)
  {
    return 2*n + 1;
  }

  idx right(idx n)
  {
    return 2*n + 2;
  }

   
} 
