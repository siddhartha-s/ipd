#include "Queue.h"

#include <UnitTest++/UnitTest++.h>

TEST(Size0)
{
    Queue queue;

    CHECK_EQUAL(0, queue.size());
}

TEST(Enq)
{
    Queue queue;

    queue.enqueue("a");
    CHECK_EQUAL(1, queue.size());
    queue.enqueue("b");
    CHECK_EQUAL(2, queue.size());
    queue.enqueue("c");
    CHECK_EQUAL(3, queue.size());
}

TEST(EnqDeq)
{
    Queue queue;

    queue.enqueue("a");
    queue.enqueue("b");
    queue.enqueue("c");

    CHECK_EQUAL("a", queue.dequeue());
    CHECK_EQUAL(2, queue.size());

    queue.enqueue("d");
    CHECK_EQUAL(3, queue.size());
    CHECK_EQUAL("b", queue.dequeue());
    CHECK_EQUAL(2, queue.size());
    CHECK_EQUAL("c", queue.dequeue());
    CHECK_EQUAL("d", queue.dequeue());
    CHECK_EQUAL(0, queue.size());
}

TEST(DeqEmpty)
{
    Queue queue;

    CHECK_THROW(queue.dequeue(), std::logic_error);
}
