#ifndef MTTI2T_BINARIZER_H_
#define MTTI2T_BINARIZER_H_

#include <cassert>
#include <cstdint>
#include <optional>
#include <utility>

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
  
    // 15x15 or 25x25 usually, 0.2-0.5 usually
    class SauvolaThreshold {
      struct Integrals {
        std::uint64_t sum;
        std::uint64_t sum_squared;
      };

      int width_radius_;
      int height_radius_;
      double K_;
      bool use_integral_images_;

    public:
      Pointer < std::uint8_t >  operator () (std::uint8_t * grayscale_data, int width, int height) noexcept;

      SauvolaThreshold(int width_radius, int height_radius, double K, bool use_integral_images) noexcept {
        assert(width_radius > 0 && height_radius > 0);

        width_radius_ = width_radius;
        height_radius_ = height_radius;
        K_ = K;
        use_integral_images_ = use_integral_images;
      }

    private:
      bool WithIntegralImages(std::uint8_t * grayscale_data, std::uint8_t * binary_data, int width, int height) noexcept;
      bool WithoutIntegralImages(std::uint8_t * grayscale_data, std::uint8_t * binary_data, int width, int height) noexcept;
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