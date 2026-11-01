#include <iostream>
#include "SensorProcessor.h"
#include <cassert>
#include <thread>
#include <chrono>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

void produceEvents(std::queue<SensorEvent>& event_queue, std::mutex& queue_mutex) {
    for (int i = 0; i < 10; i++) {
        SensorEvent event{
            1000 + static_cast<std::uint64_t>(i),
            1,
            20.0f + (i % 10),
            101.0f,
            0.02f
        };

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            event_queue.push(event);
        } // Unlock here
    }
}

void consumeEvents(
    std::queue<SensorEvent>& event_queue,
    SensorProcessor& processor,
    std::mutex& queue_mutex,
    std::condition_variable& queue_cv,
    const bool& producer_finished
) {
    while (true) {
        SensorEvent event;

        {
            //std::lock_guard<std::mutex> lock(queue_mutex);
            std::unique_lock<std::mutex> lock(queue_mutex);

            queue_cv.wait(lock, [&] {
                return !event_queue.empty() || producer_finished;
            });

            if (event_queue.empty()) {
                break;
            }

            event = event_queue.front();
            event_queue.pop();
        } // Mutex unlocks here

        processor.addEvent(event);
    }
}



int main() {
    std::cout << "MAIN STARTED\n";

    try {
        std::cout << "CREATING PROCESSOR\n";

        SensorProcessor processor(2);

        std::cout << "Processor created successfully!\n";

        processor.addEvent({1001, 1, 10.0f, 101.0f, 0.01f});
        processor.addEvent({1002, 1, 20.0f, 101.0f, 0.02f});
        processor.addEvent({1003, 1, 30.0f, 101.0f, 0.03f});

        assert(processor.getAverageTemp() == 20.0f);
        assert(processor.getRollingAverage() == 25.0f);
        assert(processor.getMaxTemp() == 30.0f);
        assert(processor.getMinTemp() == 10.0f);
        std::cout << "All tests passed!\n";

        std::cout << "Overall average: "
                  << processor.getAverageTemp() << "\n";

        std::cout << "Rolling average: "
                  << processor.getRollingAverage() << "\n";

        SensorProcessor processor2(3);
        assert(processor2.getAverageTemp() == 0.0f);
        assert(processor2.getRollingAverage() == 0.0f);


        SensorProcessor stream(5);
        for (int i = 0; i < 10; i++) {
            float temperature = 20.0f + i;

            SensorEvent event{
                1000 + static_cast<std::uint64_t>(i),
                1,
                temperature,
                101.0f,
                0.02f
            };

            stream.addEvent(event);

            std::cout << "New temperature: " << temperature
                    << " | Rolling average: "
                    << stream.getRollingAverage() << "\n";
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "Average temp from all readings " << stream.getAverageTemp() << "\n";


        SensorProcessor benchmark_processor(5);

        const int total_events = 1000000;
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < total_events; i++) {
                    float temperature = 20.0f + i%10;

                    SensorEvent event{
                        1000 + static_cast<std::uint64_t>(i),
                        1,
                        temperature,
                        101.0f,
                        0.02f
                    };

                    benchmark_processor.addEvent(event);
                  
                }
            
        

        auto end = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );

        std::cout << "Processed " << total_events
                << " events in " << elapsed.count()
                << " microseconds\n";
        
        double seconds = elapsed.count() / 1'000'000.0;

        double events_per_second = total_events / seconds;

        std::cout << "Throughput: "
                << events_per_second
                << " events/second\n";
        std::cout << "Average temp from all readings " << benchmark_processor.getAverageTemp() << "\n";
 /////////////////////////////////////////////////////////

        std::queue<SensorEvent> event_queue;
        std::mutex queue_mutex;
        SensorProcessor queue_processor(2);

        std::thread producer_thread(
            produceEvents,
            std::ref(event_queue),
            std::ref(queue_mutex)
        );
        // Wait for producer to finish
        producer_thread.join();
        // Process all events
        consumeEvents(event_queue, queue_processor, queue_mutex);

        std::cout << "Overall average: "
          << queue_processor.getAverageTemp() << "\n";

        std::cout << "Rolling average: "
            << queue_processor.getRollingAverage() << "\n";
        
        std::cout << "Events remaining: " << event_queue.size() <<"\n";

    }
    catch (const std::invalid_argument& error) {
        std::cout << "ERROR: " << error.what() << "\n";
    }

    return 0;
}