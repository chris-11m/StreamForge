#include "streamforge/sensor_simulator.hpp"
#include <stdexcept>
#include <random>

namespace streamforge {

SensorSimulator::SensorSimulator(
    std::uint32_t sensor_id,
    double anomaly_probability
)
    : sensor_id_(sensor_id),
      rng_(std::random_device{}()),
      anomaly_dist_(0.01)
{
    if (!(anomaly_probability >= 0.0 &&
          anomaly_probability <= 1.0)) {
        throw std::invalid_argument(
            "Anomaly probability must be between 0 and 1."
        );
    }

    anomaly_dist_ = std::bernoulli_distribution(
        anomaly_probability
    );
}


SensorEvent SensorSimulator::generateEvent(std::uint64_t timestamp)
{
    std::normal_distribution<float> temperature_dist(25.0f, 2.0f);
    std::normal_distribution<float> pressure_dist(101.0f, 1.0f);
    std::normal_distribution<float> vibration_dist(0.02f, 0.005f);
    //std::bernoulli_distribution anomaly_dist(0.01);

    SensorEvent event{
        timestamp,
        sensor_id_,
        temperature_dist(rng_),
        pressure_dist(rng_),
        vibration_dist(rng_)
    };

    if (anomaly_dist_(rng_)) {
        event.temperature += 60.0f;
        event.pressure += 50.0f;
        event.vibration += 0.3f;
    }

    return event;
}




}

