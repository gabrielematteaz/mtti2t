#ifndef MTTI2T_DENSITY_CALCULATOR_H_
#define MTTI2T_DENSITY_CALCULATOR_H_

#include <cstdint>

namespace mtti2t {
  class MeanDensityCalculator {
    int width_blocks_;
    int height_blocks_;

  public:
    std::uint8_t * operator () (std::uint8_t * data, int width, int height) noexcept;

    MeanDensityCalculator(int width_blocks, int height_blocks) noexcept {
      width_blocks_ = width_blocks;
      height_blocks_ = height_blocks;
    }
  };
}

#endif