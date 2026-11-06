#include <iostream>
#include "SensorProcessor.h"
#include "streamforge/bounded_queue.hpp"
#include <cassert>
#include <thread>
#include <chrono>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <syncstream>
#include "streamforge/sensor_simulator.hpp"
#include "streamforge/anomaly_detector.hpp"
#include "streamforge/event_json.hpp"

void produceEvents(streamforge::BoundedQueue<streamforge::SensorEvent>& event_queue) {

    streamforge::SensorSimulator simulator(1);

    for (int i = 0; i < 1000; ++i) {

        auto event = simulator.generateEvent(
            1000 + static_cast<std::uint64_t>(i)
        );

        if (!event_queue.push(event)) {
            break;
        }
    }

    event_queue.close();
}



void consumeEvents(
    streamforge::BoundedQueue<streamforge::SensorEvent>& event_queue,
    SensorProcessor& processor
) {
    streamforge::SensorEvent event;
    streamforge::AnomalyDetector detector;

    std::size_t anomaly_count = 0;
    std::size_t event_count = 0;

    while (event_queue.pop(event)) {
        processor.addEvent(event);

        bool anomaly = detector.isAnomaly(event);

        if (anomaly) {
            ++anomaly_count;
        }

        // Print the first 5 events as JSON.
        if (event_count < 5) {
            std::cout << streamforge::eventToJson(event, anomaly)
                      << '\n';
        }

        ++event_count;
    }

    std::cout << "Detected anomalies: "
              << anomaly_count << '\n';
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

            streamforge::SensorEvent event{
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

                    streamforge::SensorEvent event{
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


        /////////////////////////////

        
        // Create a bounded queue with capacity 10.
        constexpr std::size_t max_queue_size = 10;

        streamforge::BoundedQueue<streamforge::SensorEvent>
            event_queue(max_queue_size);

        SensorProcessor queue_processor(2);

        // Start measuring the pipeline.
        auto pipeline_start = std::chrono::steady_clock::now();

        // Start producer and consumer threads.
        std::thread producer_thread(
            produceEvents,
            std::ref(event_queue)
        );

        std::thread consumer_thread(
            consumeEvents,
            std::ref(event_queue),
            std::ref(queue_processor)
        );

        // Wait for both threads to finish.
        producer_thread.join();
        consumer_thread.join();

        // Stop measuring the pipeline.
        auto pipeline_end = std::chrono::steady_clock::now();

        double pipeline_seconds =
            std::chrono::duration<double>(
                pipeline_end - pipeline_start
            ).count();

        double pipeline_throughput = 1000.0 / pipeline_seconds;

        std::cout << "Pipeline runtime: "
                  << pipeline_seconds << " seconds\n";

        std::cout << "Pipeline throughput: "
                  << pipeline_throughput << " events/second\n";

        std::cout << "Overall average: "
                  << queue_processor.getAverageTemp() << "\n";

        std::cout << "Rolling average: "
                  << queue_processor.getRollingAverage() << "\n";

        
        std::cout << "Peak queue size: "
                << event_queue.peakQueueSize() << "\n";

        std::cout << "Producer waits: "
                << event_queue.producerWaitCount() << "\n";

        std::cout << "Total producer wait time: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                        event_queue.producerWaitTime()
                    ).count()
                << " ms\n";
        
        std::cout << "Total producer wait time: "
          << std::chrono::duration<double, std::micro>(
                 event_queue.producerWaitTime()
             ).count()
          << " microseconds\n";
        
        


      

    }
    catch (const std::invalid_argument& error) {
        std::cout << "ERROR: " << error.what() << "\n";
    }

    return 0;
}