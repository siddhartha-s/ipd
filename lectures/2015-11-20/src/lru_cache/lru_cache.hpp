#include "../hash_table/hash_table.hpp"
#include "../linked_structures/deque.hpp"

namespace lru
{

  template <typename T>
  struct lru_entry
  {
    typename deque::deque<T>::loc dq_loc;
    T val;
  };
  
  template <typename T>
  class lru_cache
  {

  public:

    void put(int, T);
    T get(int);
    bool hasKey(int);

    lru_cache(size_t);
    lru_cache();
    
  private:

    hash_table::hash_table<lru_entry<T>> table;
    deque::deque<int> dq;
    size_t size;
    size_t max_size;
    void evict();

  };

  template <typename T>
  lru_cache<T>::lru_cache()
  {
    lru_cache(16);
  }

  template <typename T>
  lru_cache<T>::lru_cache(size_t size) : table{size * 2}, max_size{size}, size{0} { }

  template <typename T>
  void lru_cache<T>::put(int key, T val)
  {
    if (table.hasKey(key)) {
      dq.moveToFront(table.get(key).dq_loc);
    } else {
      if (size + 1 > max_size) {
	evict();
      }
      table.put(key, lru_entry<T>{dq.addFirst(key), val});
      size++;
    }
  }

  template <typename T>
  T lru_cache<T>::get(int key)
  {
    lru_entry<T> entry = table.get(key);
    dq.moveToFront(entry.dq_loc);
    return entry.val;
  }

  template <typename T>
  bool lru_cache<T>::hasKey(int key)
  {
    return table.hasKey(key);
  }

  template <typename T>
  void lru_cache<T>::evict()
  {
    int key = dq.removeLast();
    table.remove(key);
    size--;
  }

} // namespace lru
