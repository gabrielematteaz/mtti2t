#ifndef MTTI2T_BINARIZER_H_
#define MTTI2T_BINARIZER_H_

#include <cstdint>

namespace mtti2t {
  class MeanBinarizer {
    int width_blocks_;
    int height_blocks_;

  public:
    bool operator () (std::uint8_t * data, int width, int height) noexcept;

    MeanBinarizer(int width_blocks, int height_blocks) noexcept {
      width_blocks_ = width_blocks;
      height_blocks_ = height_blocks;
    }
  };

  class ThresholdBinarizer {
    int threshold_;

  public:
    bool operator () (std::uint8_t * data, int width, int height) noexcept;

    ThresholdBinarizer(int threshold) noexcept {
      threshold_ = threshold;
    }
  };
}

#endif