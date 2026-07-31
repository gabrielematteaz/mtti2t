#ifndef MTTI2T_DISTANCE_CALCULATOR_H_
#define MTTI2T_DISTANCE_CALCULATOR_H_

#include <cstdint>

namespace mtti2t {
  class DistanceCalculator {
  public:
    static int Difference(std::uint8_t * data, std::uint8_t const* reference, int width, int height) noexcept;
  };
}

#endif