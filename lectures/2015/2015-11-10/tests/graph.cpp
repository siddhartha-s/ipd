#include "graph/graph.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>

namespace graph
{

using std::string;
using gr   = graph<string>;
using node = gr::node;

TEST(Create)
{
    graph<int> g;
    CHECK_EQUAL(node::INVALID, g.last_node());
}

TEST(CreateAlloc)
{
    graph<int> g(100u);
    CHECK_EQUAL(node::INVALID, g.last_node());
}

TEST(EmptyGraph)
{
    gr g;
    CHECK_EQUAL(0u, g.size());
}

TEST(AddNode)
{
    gr g;
    g.add_node("hello");
    CHECK_EQUAL(1u, g.size());
}

TEST(AddEdge)
{
    gr g;
    node n1 = g.add_node("hello");
    node n2 = g.add_node("goodbye");

    CHECK_EQUAL(2u, g.size());
    CHECK_EQUAL(n2, g.last_node());

    g.add_edge(n1, n2, 8);

    CHECK_EQUAL(8.0, g.get_weight(n1, n2));
    CHECK_EQUAL(INFINITY, g.get_weight(n2, n1));
}

TEST(Bigger)
{
    gr g;
    node n1 = g.add_node("foo");
    node n2 = g.add_node("bar");
    node n3 = g.add_node("baz");
    node n4 = g.add_node("qux");
    node n5 = g.add_node("quux");

    CHECK_EQUAL(5u, g.size());

    g.add_edge(n1, n2, 8);
    g.add_edge(n1, n3, 2);
    g.add_edge(n1, n4, 7);
    g.add_edge(n5, n4, 7);

    CHECK_EQUAL(INFINITY, g.get_weight(n1, n1));
    CHECK_EQUAL(8.0, g.get_weight(n1, n2));
    CHECK_EQUAL(2.0, g.get_weight(n1, n3));
    CHECK_EQUAL(7.0, g.get_weight(n1, n4));
    CHECK_EQUAL(INFINITY, g.get_weight(n1, n5));

    const std::vector<node> succs{n2, n3, n4};
    CHECK(succs == g.get_successors(n1));
}

}  // namespace graph

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
