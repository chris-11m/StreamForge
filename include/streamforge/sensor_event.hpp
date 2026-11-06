#pragma once

#include <cstdint>

namespace streamforge {

struct SensorEvent {
    std::uint64_t timestamp;
    std::uint32_t sensor_id;
    float temperature;
    float pressure;
    float vibration;
};

} // namespace streamforge