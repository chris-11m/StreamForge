#pragma once

#include <cstdint>

struct SensorEvent {
    std::uint64_t timestamp;
    std::uint32_t sensor_id;
    float temperature;
    float pressure;
    float vibration;
};