#pragma once

//#include "SensorEvent.h"
#include <streamforge/sensor_event.hpp>
#include <vector>
#include <deque>
#include <cstddef>


class SensorProcessor {
private:
    float temperature_sum = 0.0f;
    int event_count = 0;
    float min_temperature = 0.0f;
    float max_temperature = 0.0f;

    std::deque<float> recent_temperatures;
    float rolling_sum = 0.0f;
    std::size_t window_size = 3;

    void addRecentTemperature(float temperature);

public:
    explicit SensorProcessor(std::size_t size = 3);

    void addEvent(const streamforge::SensorEvent& event);

    float getRollingAverage() const;
    float getAverageTemp() const;
    float getMaxTemp() const;
    float getMinTemp() const;

    float calculateAverageTemp(const std::vector<streamforge::SensorEvent>& events) const;
    float calculateMaxTemp(const std::vector<streamforge::SensorEvent>& events) const;
    float calculateMinTemp(const std::vector<streamforge::SensorEvent>& events) const;
    
};