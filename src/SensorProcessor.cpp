#include "SensorProcessor.h"
#include <stdexcept>


SensorProcessor::SensorProcessor(std::size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Window size must be greater than zero!");
    }
    window_size = size;
}


void SensorProcessor::addRecentTemperature(float temperature) {
    recent_temperatures.push_back(temperature);
    rolling_sum += temperature;

    if (recent_temperatures.size() > window_size) {
        rolling_sum -= recent_temperatures.front();
        recent_temperatures.pop_front();
    }
}

void SensorProcessor::addEvent(const SensorEvent& event) {
    if (event_count == 0) {
        min_temperature = event.temperature;
        max_temperature = event.temperature;
    } else {
        if (event.temperature > max_temperature) {
            max_temperature = event.temperature;
        }
        if (event.temperature < min_temperature) {
            min_temperature = event.temperature;
        }
    }
    temperature_sum += event.temperature;
    event_count += 1;
    addRecentTemperature(event.temperature);
}


float SensorProcessor::getRollingAverage() const {
    if (recent_temperatures.empty()) {
        return 0.0f;
    }

    return rolling_sum / recent_temperatures.size();
}

float SensorProcessor::getAverageTemp() const {
    if (event_count == 0) {
        return 0.0f;
    }

    return temperature_sum / event_count;
}

float SensorProcessor::getMaxTemp() const{
    if (event_count == 0) {
        return 0.0f;
    }
    return max_temperature;
}

float SensorProcessor::getMinTemp() const{
    if (event_count == 0) {
        return 0.0f;
    }
    return min_temperature;
}

float SensorProcessor::calculateAverageTemp(const std::vector<SensorEvent>& events) const {
    if (events.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;

    for (const SensorEvent& event : events) {
        sum += event.temperature;
    }

    return sum / events.size();
}

float SensorProcessor::calculateMaxTemp(
    const std::vector<SensorEvent>& events
) const {
    if (events.empty()) return 0.0f;

    float max_temp = events[0].temperature;

    for (const SensorEvent& event : events) {
        if (event.temperature > max_temp) max_temp = event.temperature;

    }

    return max_temp;
}

float SensorProcessor::calculateMinTemp(
    const std::vector<SensorEvent>& events
) const {
    if (events.empty()) return 0.0f;

    float min_temp = events[0].temperature;

    for (const SensorEvent& event : events) {
        if (event.temperature < min_temp) min_temp = event.temperature;

    }

    return min_temp;
}


