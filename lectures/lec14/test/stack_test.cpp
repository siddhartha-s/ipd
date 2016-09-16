#include "Stack.h"

#include <UnitTest++/UnitTest++.h>
#include <stdexcept>

TEST(Size0)
{
    Stack stack;

    CHECK_EQUAL(0, stack.size());
}

TEST(Push)
{
    Stack stack;

    stack.push(1.0);
    CHECK_EQUAL(1, stack.size());
    stack.push(2.0);
    CHECK_EQUAL(2, stack.size());
    stack.push(3.0);
    CHECK_EQUAL(3, stack.size());
}

TEST(PushPop)
{
    Stack stack;

    stack.push(1.0);
    stack.push(2.0);
    stack.push(3.0);

    CHECK_EQUAL(3.0, stack.pop());
    CHECK_EQUAL(2, stack.size());

    stack.push(4.0);
    CHECK_EQUAL(3, stack.size());
    CHECK_EQUAL(4.0, stack.pop());
    CHECK_EQUAL(2, stack.size());
    CHECK_EQUAL(2.0, stack.pop());
    CHECK_EQUAL(1.0, stack.pop());
    CHECK_EQUAL(0, stack.size());
}

TEST(PopEmpty)
{
    Stack stack;

    CHECK_THROW(stack.pop(), std::logic_error);
}