#pragma once

#include "streamforge/sensor_event.hpp"
#include <random>
#include <cstdint>

namespace streamforge {

class SensorSimulator {
public:
    explicit SensorSimulator(std::uint32_t sensor_id, double anomaly_probability=0.01);

    SensorEvent generateEvent(std::uint64_t timestamp);

private:
    std::uint32_t sensor_id_;
    std::mt19937 rng_;
    std::bernoulli_distribution anomaly_dist_;
};

}