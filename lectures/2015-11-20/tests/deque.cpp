#include "linked_structures/deque.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>

namespace deque
{

   
  TEST(Create)
  {
    deque<std::string> d;
  }

  TEST(Small)
  {
    deque<std::string> d;
    CHECK(d.isEmpty());
    d.addFirst("a");
    CHECK_EQUAL("a", d.removeFirst());
    CHECK(d.isEmpty());
    d.addFirst("a");
    CHECK("a" == d.removeLast()); 
    CHECK(d.isEmpty());
    d.addLast("z"); 
    CHECK_EQUAL("z", d.removeLast()); 
    CHECK(d.isEmpty());
    d.addLast("z");
    CHECK_EQUAL("z", d.removeFirst());
    CHECK(d.isEmpty());
  }

  TEST(MEDIUM)
  {
    deque<std::string> d;
    d.addFirst("b");
    d.addFirst("a");
    d.addLast("c");
    d.addLast("d");
    CHECK_EQUAL("a", d.removeFirst());
    CHECK_EQUAL("b", d.removeFirst());
    CHECK_EQUAL("c", d.removeFirst());
    CHECK_EQUAL("d", d.removeFirst());
    CHECK(d.isEmpty());
    d.addLast("c");
    d.addLast("d"); 
    d.addFirst("b");
    d.addFirst("a");
    CHECK_EQUAL("d", d.removeLast());
    CHECK_EQUAL("c", d.removeLast());
    CHECK_EQUAL("b", d.removeLast());
    CHECK_EQUAL("a", d.removeLast());
    CHECK(d.isEmpty());
  }

  TEST(LARGER)
  {
    int numels = 100;
    deque<int> d;
    for(int i = 0; i < numels; i++)
      d.addFirst(i);
    for(int i = 0; i < numels; i++)
      CHECK_EQUAL(i, d.removeLast());
    CHECK(d.isEmpty());
    for(int i = 0; i < numels; i++)
      d.addLast(i);
    for(int i = 0; i < numels; i++)
      CHECK_EQUAL(i, d.removeFirst());
    CHECK(d.isEmpty());
    for(int i = 0; i < numels; i++) {
      if (i % 2 == 0)
	d.addFirst(i);
      else
	d.addLast(i);
    }
    for(int i = 0; i < numels; i++) {
      if (i % 2 == 0)
	d.removeLast();
      else
	d.removeFirst();
    }
    CHECK(d.isEmpty());
  }

  TEST(REFS)
  {
    deque<std::string> d;
    deque<std::string>::loc b = d.addFirst("b");
    d.addFirst("a");
    d.addLast("c");
    CHECK_EQUAL("b", b->first);
    d.removeLast();
    CHECK_EQUAL("b", b->first);
    d.removeFirst();
    CHECK_EQUAL("b", b->first);
    d.removeFirst();
    CHECK(d.isEmpty());
    b = d.addFirst("b");
    d.addFirst("a");
    d.addLast("c");
    CHECK_EQUAL("b", d.remove(b));
    CHECK_EQUAL("a", d.removeFirst());
    CHECK_EQUAL("c", d.removeFirst());
    CHECK(d.isEmpty());
  }

  TEST(Remove)
  {
    
    deque<std::string> d;
    deque<std::string>::loc x = d.addFirst("x");
    CHECK_EQUAL("x", d.remove(x));
    CHECK(d.isEmpty());
  }

  TEST(MoveToEnd)
  {
    deque<std::string> d;
    deque<std::string>::loc x = d.addFirst("x");
    d.moveToEnd(x);
    CHECK_EQUAL("x",d.removeLast());
    CHECK(d.isEmpty());
    deque<std::string>::loc b = d.addFirst("b");
    d.addFirst("a");
    d.moveToEnd(b);
    CHECK_EQUAL("b",d.removeLast());
    CHECK_EQUAL("a",d.removeLast());
    CHECK(d.isEmpty());
    d.addFirst("a");
    b = d.addFirst("b");
    d.addFirst("c");
    d.moveToEnd(b);
    CHECK_EQUAL("b",d.removeLast());
    CHECK_EQUAL("a",d.removeLast());
    CHECK_EQUAL("c",d.removeLast());
    CHECK(d.isEmpty());
  }

  TEST(MoveToEnd2)
  {
    deque<int> d;
    deque<int>::loc x = d.addFirst(3);
    deque<int>::loc a = d.addFirst(1);
    deque<int>::loc b = d.addLast(2);
    d.moveToEnd(x);
    CHECK_EQUAL(3, d.removeLast());
    d.moveToEnd(a);
    CHECK_EQUAL(1, d.removeLast());
    d.moveToEnd(b);
    CHECK_EQUAL(2, d.removeLast());
    CHECK(d.isEmpty());
  }
  
  TEST(MoveToFront)
  {
    deque<int> d;
    deque<int>::loc three = d.addFirst(3);
    deque<int>::loc one = d.addFirst(1);
    deque<int>::loc two = d.addLast(2);
    d.moveToFront(two);
    CHECK_EQUAL(2, d.removeFirst());
    d.moveToFront(three);
    CHECK_EQUAL(3, d.removeFirst());
    d.moveToFront(one);
    CHECK_EQUAL(1, d.removeFirst());
    CHECK(d.isEmpty());
  }

}  // namespace deque

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
