#include "graph/graph.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>

namespace graph
{

using std::string;
using gr = graph<string>;
using node = gr::node;

  TEST(MST)
  {
    gr g;
    node n1 = g.add_node("A");
    node n2 = g.add_node("B");
    node n3 = g.add_node("C");
    
    g.add_edge(n1, n2, 1);
    g.add_edge(n2, n3, 2);
    g.add_edge(n3, n1, 3);
    
    gr gmst = mstk(g);

    CHECK_EQUAL(1.0, gmst.get_weight(n1, n2));
    CHECK_EQUAL(2.0, gmst.get_weight(n2, n3));
    // this edge isn't in the spanning tree:
    CHECK_EQUAL(gr::INFINITY, gmst.get_weight(n3, n1));
  }

}  // namespace graph

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
