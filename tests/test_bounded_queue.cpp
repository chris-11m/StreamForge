
#include "streamforge/bounded_queue.hpp"

#include <cassert>
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>


void test_close_unblocks_producer() {
    streamforge::BoundedQueue<int> queue(1);

    // Fill the queue.
    assert(queue.push(10));

    bool second_push_succeeded = true;

    std::thread producer([&] {
        // Queue is full, so this waits.
        second_push_succeeded = queue.push(20);
    });

    // Give the producer time to reach push().
    std::this_thread::sleep_for(
        std::chrono::milliseconds(50)
    );

    // Close the queue while the producer is waiting.
    queue.close();

    producer.join();

    // The blocked push should return false.
    assert(!second_push_succeeded);

    int value = 0;

    // The original event should still be available.
    assert(queue.pop(value));
    assert(value == 10);

    // Queue is now closed and empty.
    assert(!queue.pop(value));

    std::cout << "Close unblocks producer test passed!\n";
}



int main() {
    streamforge::BoundedQueue<int> queue(2);

    std::thread producer([&] {
        for (int i = 0; i < 100; ++i) {
            bool success = queue.push(i);
            assert(success);
        }

        queue.close();
    });

    int received_count = 0;
    int value = 0;

    while (queue.pop(value)) {
        assert(value == received_count);
        ++received_count;
    }

    producer.join();

    assert(received_count == 100);

    std::cout << "BoundedQueue test passed!\n";

    test_close_unblocks_producer();
}
