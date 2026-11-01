#pragma once
#include <cstdint>
namespace streamforge {
struct Event {
    std::uint64_t timestamp_ns;
    std::uint32_t instrument_id;
    double price;
    std::uint32_t quantity;
};
inline double notional(const Event& event) noexcept {
    return event.price * static_cast<double>(event.quantity);
}
} // namespace streamforge
