#include "streamforge/event_json.hpp"
#include "streamforge/anomaly_detector.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    streamforge::SensorEvent event{
        12345, 7, 45.0f, 101.0f, 0.02f
    };

    // Default threshold: 45 is normal.
    streamforge::AnomalyDetector default_detector;
    assert(!default_detector.isAnomaly(event));

    // Custom threshold: 45 is anomalous.
    streamforge::AnomalyDetector custom_detector(
        40.0f, 130.0f, 0.15f
    );
    assert(custom_detector.isAnomaly(event));

    std::string json = streamforge::eventToJson(event, true);

    assert(json.find("\"timestamp\":12345") != std::string::npos);
    assert(json.find("\"sensor_id\":7") != std::string::npos);
    assert(json.find("\"anomaly\":true") != std::string::npos);

    std::cout << "Event JSON tests passed!\n";
    return 0;
}