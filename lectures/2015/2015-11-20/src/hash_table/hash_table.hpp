#include <cstddef>
#include <vector>

namespace hash_table
{

  // an entry in a hash table
  template<typename T>
  struct entry
  {
    int key;
    T value;
    bool empty;
    bool deleted;

    // default constructor makes an empty entry:
    entry() : empty(true) {}
    entry(int key, T value, bool empty, bool deleted) 
      : key(key), value(value), empty(empty), deleted(deleted) {}
  };

  // a hash table is a vector of entries
  // invariant: the number of active (non empty/deleted) entries
  // is less than half the size of the vector
  template<typename T>
  class hash_table {

  public:

    // make a new table of default size
    hash_table();
    // make a new table, with specified size and
    // empty entries
    hash_table(size_t);

    // add a key/value pair to the table
    void put(int, T);
    // for a given key, get the value
    T get(int);
    // remove a key/value pair from the table
    void remove(int);
    // check if a key is in the table
    bool hasKey(int);

  private:
      
    std::vector<entry<T>> table;
						
    // the number of active entries
    size_t num_entries;
    // reallocate a new table, double the size
    void resize();
    // given a hash code, turn it into an index into the table
    size_t hash_index(int);
    // construct an empty entry
    entry<T> empty;
    // get a reference to an entry in the table
    entry<T>& get_entry(int);
  };

  template<typename T>
  hash_table<T>::hash_table()
  {
    hash_table(16);
  }

  template<typename T>
  hash_table<T>::hash_table(size_t size)
    : table(size), num_entries(0) 
  { }

  template<typename T>
  void hash_table<T>::put(int key, T val) 
  {
    if (num_entries + 1 > (table.size() / 2))
      resize();
    size_t i = hash_index(key);
    while (!(table[i].empty || table[i].deleted))
      i = (i + 1) % table.size();
    table[i] = entry<T>{key, val, false, false};
    num_entries++;
  }

  template<typename T>
  entry<T>& hash_table<T>::get_entry(int key)
  {
    size_t orig_index = hash_index(key);
    size_t i = orig_index;
    do {
      if (table[i].empty) {
	return table[i];
      } else if (table[i].deleted || table[i].key != key) {
	i = (i + 1) % table.size(); 
      } else {
	return table[i];
      }
    } while (i != orig_index);
    return empty;
  }

  template<typename T>
  T hash_table<T>::get(int key)
  {
    auto& e = get_entry(key);
    if (e.empty) {
      throw std::invalid_argument("key not found");      
    } else {
      return e.value;
    }
  }

  template<typename T>
  bool hash_table<T>::hasKey(int key)
  {
    auto& e = get_entry(key);
    return !e.empty;
  }

  template<typename T>
  void hash_table<T>::remove(int key)
  {
    auto& e = get_entry(key);
    if (e.empty) {
      throw std::invalid_argument("key not found");      
    } else {
      e.deleted = true;
      num_entries--;
    }
  }
  
  template<typename T>
  void hash_table<T>::resize()
  {
    size_t newsize = table.size() * 2;
    std::vector<entry<T>> oldtable = table;
    std::vector<entry<T>> newtable{newsize};
    table = newtable;
    for (auto e : oldtable) {
      if (!(e.empty || e.deleted)) {
	put(e.key, e.value);
      }
    }
  }

  template<typename T>
  size_t hash_table<T>::hash_index(int key)
  {
    return ((12u * key + 27u) % 17942673) % table.size();
  }

}
