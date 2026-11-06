#include "streamforge/anomaly_detector.hpp"

namespace streamforge {

AnomalyDetector::AnomalyDetector(
    float temperature_threshold,
    float pressure_threshold,
    float vibration_threshold
)
    : temperature_threshold_(temperature_threshold),
      pressure_threshold_(pressure_threshold),
      vibration_threshold_(vibration_threshold)
{
}

bool AnomalyDetector::isAnomaly(const SensorEvent& event) const {
    return event.temperature > temperature_threshold_ ||
           event.pressure > pressure_threshold_ ||
           event.vibration > vibration_threshold_;
}

}