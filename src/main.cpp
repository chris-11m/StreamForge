#include <iostream>
#include "streamforge/event.hpp"




int main() {
    const streamforge::Event event{1000, 42, 101.25, 8};
    std::cout << "instrument=" << event.instrument_id
              << " notional=" << streamforge::notional(event) << '\n';
}
