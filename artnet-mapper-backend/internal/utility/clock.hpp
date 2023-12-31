//
// Created by broosegoose on 7/9/23.
//

#ifndef UTILS_CLOCK_HPP
#define UTILS_CLOCK_HPP

#include <chrono>
using namespace std::literals;

namespace utility {
    typedef std::chrono::duration<long int> LongDuration;
    typedef std::chrono::duration<double> Duration;
    typedef std::chrono::duration<float, std::chrono::seconds::period> QuickDuration;
    typedef std::chrono::high_resolution_clock Clock;
}

#endif //UTILS_CLOCK_HPP
