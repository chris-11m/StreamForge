#include "streamforge/sensor_simulator.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>

int main() {
    // Test 1: Sensor ID and timestamp are preserved.
    streamforge::SensorSimulator normal_sensor(42, 0.0);

    auto event = normal_sensor.generateEvent(12345);

    assert(event.sensor_id == 42);
    assert(event.timestamp == 12345);

    // Test 2: Invalid probability is rejected.
    bool threw = false;

    try {
        streamforge::SensorSimulator invalid_sensor(1, 1.5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);

    // Test 3: With 100% anomaly probability,
    // injected readings should be far above normal.
    streamforge::SensorSimulator anomaly_sensor(7, 1.0);

    auto anomaly = anomaly_sensor.generateEvent(999);

    assert(anomaly.sensor_id == 7);
    assert(anomaly.timestamp == 999);
    assert(anomaly.temperature > 60.0f);
    assert(anomaly.pressure > 130.0f);
    assert(anomaly.vibration > 0.15f);

    std::cout << "Sensor simulator tests passed!\n";
}