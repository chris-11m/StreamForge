#include "streamforge/anomaly_detector.hpp"

#include <cassert>
#include <iostream>

int main() {
    streamforge::AnomalyDetector detector;

    // Normal sensor reading
    streamforge::SensorEvent normal{
        1000, 1, 25.0f, 101.0f, 0.02f
    };

    assert(!detector.isAnomaly(normal));

    // Temperature anomaly
    streamforge::SensorEvent high_temp{
        1001, 1, 85.0f, 101.0f, 0.02f
    };

    assert(detector.isAnomaly(high_temp));

    // Pressure anomaly
    streamforge::SensorEvent high_pressure{
        1002, 1, 25.0f, 150.0f, 0.02f
    };

    assert(detector.isAnomaly(high_pressure));

    // Vibration anomaly
    streamforge::SensorEvent high_vibration{
        1003, 1, 25.0f, 101.0f, 0.30f
    };

    assert(detector.isAnomaly(high_vibration));

    // Boundary test: exactly at thresholds
    streamforge::SensorEvent boundary{
        1004, 1, 60.0f, 130.0f, 0.15f
    };

    assert(!detector.isAnomaly(boundary));

    std::cout << "Anomaly detector tests passed!\n";
    return 0;
}