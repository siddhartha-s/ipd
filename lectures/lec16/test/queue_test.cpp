#include "UP_queue.h"
#include <UnitTest++/UnitTest++.h>

#include <stdexcept>

TEST(Empty) {
    Queue<int> q;
    CHECK(q.empty());
}

TEST(NotEmpty) {
    Queue<int> q;
    q.enqueue(1);
    CHECK(! q.empty());
}

TEST(EnqueueEnqueue) {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
}

TEST(EnqueueDequeue) {
    Queue<int> q;
    q.enqueue(1);
    CHECK_EQUAL(1, q.dequeue());
}

TEST(EnqueueEnqueueDequeueDequeue) {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    CHECK_EQUAL(1, q.dequeue());
    CHECK_EQUAL(2, q.dequeue());
    CHECK_EQUAL(3, q.dequeue());
    CHECK_EQUAL(4, q.dequeue());
    CHECK_THROW(q.dequeue(), std::logic_error);
}

TEST(EnqueueDequeueMore) {
    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    CHECK_EQUAL(1, q.dequeue());
    q.enqueue(3);
    q.enqueue(4);
    CHECK_EQUAL(2, q.dequeue());
    CHECK_EQUAL(3, q.dequeue());
    CHECK_EQUAL(4, q.dequeue());
    CHECK_THROW(q.dequeue(), std::logic_error);
    q.enqueue(5);
    CHECK_EQUAL(5, q.dequeue());
    q.enqueue(6);
    q.enqueue(7);
    CHECK_EQUAL(6, q.dequeue());
    CHECK_EQUAL(7, q.dequeue());
    CHECK_THROW(q.dequeue(), std::logic_error);
}
