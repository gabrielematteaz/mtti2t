#ifndef MTTI2T_BINARIZER_H_
#define MTTI2T_BINARIZER_H_

#include <cassert>
#include <cstdint>

#include "data_structures\pointer.h"

namespace mtti2t {
  namespace binarizers {
    // add logic to invert image to always have text white
    // add logic to preserve e.g. local threshold (optimization)
    // add standard factories with some settings

    // otsu threshold
    // niblack threshold OK + integral image
    // sauvola threshold OK + integral image
    // super noisy ->
    // wolf algorithm
    // NICK algorithm
    // feng method

    class NiblackThreshold {
      // 15 or 25 usually
      int window_width_;
      int window_height_;
      double K_; // between 0.2 and 0.5 usually (>> means more sensitive to text and noise ???)

    public:
      Pointer < std::uint8_t > operator () (std::uint8_t * grayscale_data, int width, int height) noexcept;

      NiblackThreshold(int window_width, int window_height, double K) noexcept {
        assert(window_width > 0 && window_height > 0);

        window_width_ = window_width;
        window_height_ = window_height;
        K_ = K;
      }
    };

    class SauvolaThreshold {
      // 15 or 25 usually
      int window_width_;
      int window_height_;
      double K_; // between 0.2 and 0.5 usually (>> means more sensitive to text and noise ???)

    public:
      Pointer < std::uint8_t >  operator () (std::uint8_t * grayscale_data, int width, int height) noexcept;

      SauvolaThreshold(int window_width, int window_height, double K) noexcept {
        assert(window_width > 0 && window_height > 0);

        window_width_ = window_width;
        window_height_ = window_height;
        K_ = K;
      }
    };

    class GlobalThreshold {
      int threshold_;

    public:
      Pointer < std::uint8_t > operator () (std::uint8_t * grayscale_data, int width, int height) noexcept;

      GlobalThreshold(int threshold) noexcept {
        threshold_ = threshold;
      }
    };
  };
}

#endif