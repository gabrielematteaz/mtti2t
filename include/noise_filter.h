#ifndef MTTI2T_NOISE_FILTER_H_
#define MTTI2T_NOISE_FILTER_H_

#include "data_structures\pointer.h"

namespace mtti2t {
  namespace noise_filters {
    // dilatation & erosion
    // opening & closing

    class Dilatation {
      int kernel_width_;
      int kernel_height_;

    public:
      Dilatation(int kernel_width, int kernel_height) noexcept {
        kernel_width_ = kernel_width;
        kernel_height_ = kernel_height;
      }

      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };

    class Erosion {
      int kernel_width_;
      int kernel_height_;

    public:
      Erosion(int kernel_width, int kernel_height) noexcept {
        kernel_width_ = kernel_width;
        kernel_height_ = kernel_height;
      }

      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };
  };
}

#endif