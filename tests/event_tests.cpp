#include <cassert>
#include "streamforge/event.hpp"
int main() {
    const streamforge::Event event{1000, 42, 101.25, 8};
    assert(streamforge::notional(event) == 810.0);
}
