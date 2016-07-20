#include "graph/graph.hpp"
#include "dijkstra/dijkstra.hpp"
#include <UnitTest++/UnitTest++.h>

#include <string>
#include <iostream>

namespace dijkstra
{

using std::string;
using gr   = ::graph::graph<string>;
using node = gr::node;

TEST(Dijkstra)
{
    gr g;
    node a = g.add_node("a");
    node b = g.add_node("b");
    node c = g.add_node("c");
    node d = g.add_node("d");
    node e = g.add_node("e");
    node f = g.add_node("f");

    g.add_edge(a, b, 6);
    g.add_edge(a, c, 1);
    g.add_edge(c, d, 1);
    g.add_edge(c, a, 3);
    g.add_edge(d, b, 1);
    g.add_edge(a, e, 5);
    g.add_edge(b, e, 5);
    g.add_edge(c, e, 5);
    g.add_edge(d, e, 5);
    g.add_edge(f, e, 5);

    auto result = sssp(g, a);

    std::vector<weight> dist{0, 3, 1, 2, 5, graph::INFINITY};
    std::vector<node> prev{node::INVALID, d, a, c, a, node::INVALID};

    CHECK(dist == std::get<0>(result));
    CHECK(prev == std::get<1>(result));
}

}  // namespace dijkstra

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
