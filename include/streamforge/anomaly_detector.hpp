#pragma once

#include "streamforge/sensor_event.hpp"

namespace streamforge {

class AnomalyDetector {
public:
    explicit AnomalyDetector(
        float temperature_threshold = 60.0f,
        float pressure_threshold = 130.0f,
        float vibration_threshold = 0.15f
    );

    bool isAnomaly(const SensorEvent& event) const;

private:
    float temperature_threshold_;
    float pressure_threshold_;
    float vibration_threshold_;
};

}