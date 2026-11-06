#pragma once

#include "streamforge/sensor_event.hpp"

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

namespace streamforge {

inline std::string eventToJson(const SensorEvent& event, bool anomaly) {
    std::ostringstream out;
    out.imbue(std::locale::classic());
    out << std::setprecision(9);

    out << "{"
        << "\"timestamp\":" << event.timestamp << ","
        << "\"sensor_id\":" << event.sensor_id << ","
        << "\"temperature\":" << event.temperature << ","
        << "\"pressure\":" << event.pressure << ","
        << "\"vibration\":" << event.vibration << ","
        << "\"anomaly\":" << (anomaly ? "true" : "false")
        << "}";

    return out.str();
}

}