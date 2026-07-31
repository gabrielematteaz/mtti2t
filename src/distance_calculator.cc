#include "density_calculator.h"
#include "distance_calculator.h"

namespace mtti2t {
  int DistanceCalculator::Difference(std::uint8_t * data, std::uint8_t const* reference, int width, int height) noexcept {
    int accumulator = 0;

    int block_count = width * height;

    for (int index = 0; index < block_count; ++index) {
      int distance = data[index] - reference[index];

      if (distance < 0) {
        distance = -distance;
      }

      accumulator = accumulator + distance;
    }

    return accumulator;
  }
}