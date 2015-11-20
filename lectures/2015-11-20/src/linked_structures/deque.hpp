#pragma once

#include <iostream>
#include <memory>

// Double ended queue

namespace deque
{

  // a link in a doubly linked list
  // owns the next element, has a raw pointer to the previous one
  template <typename T>
  struct link
  {
    using element = T;
    using pointer = std::unique_ptr<link>;

    link(element first, pointer rest)
      : first(first), rest(move(rest)), prev(nullptr)
    {}
    
    element first;
    pointer rest;
    link<T>* prev;
  };
    
  template <typename T>
  using list = typename link<T>::pointer;

  // a deque is a doubly linked list
  // with a pointer to the first and last element
  template <typename T>
  class deque
  {

  public:

    using loc = link<T>*;

    // add a new element to the beginning, return a pointer to it
    loc addFirst(T);
    // as in addFirst, but at the end
    loc addLast(T);
    // remove the first element, return its value
    T removeFirst();
    // as in removeFirst, but at the end
    T removeLast();
    // given a pointer to an element, remove it
    T remove(loc);
    // given a pointer to an element, move it to the end, return its new location
    loc moveToEnd(loc);
    // as in moveToEnd, but to the beginning
    loc moveToFront(loc);
    // check if the deque is empty
    bool isEmpty();

    deque();

  private:

    // pointer to the head (first link) in the list (owner)
    list<T> head;
    // pointer to the tail of the list (raw)
    loc tail;
  };

  template <typename T>
  deque<T>::deque() : head{nullptr}, tail{nullptr} {}

  template <typename T>
  bool deque<T>::isEmpty() 
  {
    return head == nullptr && tail == nullptr;
  }

  template <typename T>
  auto deque<T>::addFirst(T elt) -> loc
  {
    list<T> new_link = std::make_unique<link<T>>(elt, move(head));
    new_link->prev = nullptr;
    head = move(new_link);

    if (tail == nullptr) { 
      // if it was empty, set tail to head
      tail = head.get();
    } else {
      // otherwise, set prev of old head to head
      head->rest->prev = head.get();
    }
    return head.get();
  }
  
  template <typename T>
  auto deque<T>::addLast(T elt) -> loc
  {
    
    if(isEmpty()) {
      return addFirst(elt);
    } 

    list<T> new_link = std::make_unique<link<T>>(elt, nullptr);
    loc new_tail = new_link.get();

    new_link->prev = tail;
    tail->rest = move(new_link);
    tail = tail->rest.get();

    return new_tail;
  }

  template <typename T>
  T deque<T>::removeFirst()
  {
    T elt = head->first;
    head = move(head->rest);
    // if we had one element, also remove the tail
    if (head == nullptr)
      tail = nullptr;
    else
      head->prev = nullptr;
    return elt;
  }

  template <typename T>
  T deque<T>::removeLast()
  {
    if (head.get() == tail)
      return removeFirst();

    T elt = tail->first;
    tail = tail->prev;
    tail->rest = nullptr;

    return elt;
  }
  
  template <typename T>
  T deque<T>::remove(loc lnk)
  {
    if (tail == lnk) 
      return removeLast();
    else if (head.get() == lnk)
      return removeFirst();
    
    // no special cases, so we can just update the links
    lnk->rest->prev = lnk->prev;
    lnk->prev->rest = move(lnk->rest);

    return lnk->first;
  }

  template <typename T>
  auto deque<T>::moveToEnd(loc lnk) -> loc
  {
    if (lnk == tail)
      return lnk;

    T elt = lnk->first;
    remove(lnk);
    return addLast(elt);
  }

  template <typename T>
  auto deque<T>::moveToFront(loc lnk) -> loc
  {
    if (lnk == head.get())
      return lnk;
    
    T elt = lnk->first;
    remove(lnk);
    return addFirst(elt);    
  }

}  // namespace deque

