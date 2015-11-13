#include "linked_structures/bintree.hpp"
#include <UnitTest++/UnitTest++.h>

#include <vector>

namespace linked_structures
{
using node   = treenode<int>;
using handle = node::handle;

TEST(ConstructEmpty)
{
    node::create(5);
}

TEST(ConstructNonEmpty)
{
    handle t5 = node::create(5);
    handle t7 = node::create(7);
    handle t6 = node::create(6, move(t5), move(t7));
}

static void
inorder(const node* t, std::vector<int>& result)
{
    if (t != nullptr) {
        inorder(t->left(), result);
        result.push_back(t->element());
        inorder(t->right(), result);
    }
}

TEST(PreOrder)
{
    handle t =
        node::create(
          6,
          node::create(4, node::create(3), node::create(5)),
          node::create(8, node::create(7), node::create(9)));

    std::vector<int> result;
    inorder(&*t, result);

    std::vector<int> expected{3, 4, 5, 6, 7, 8, 9};
    CHECK(expected == result);
}

} // namespace linked_structures

int
main(int, const char* [])
{
    return UnitTest::RunAllTests();
}
