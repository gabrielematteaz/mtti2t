#ifndef MTTI2T_GRAYSCALE_CONVERTER_H_
#define MTTI2T_GRAYSCALE_CONVERTER_H_

#include <cstdint>

#include "BMP.h"

namespace mtti2t {
  class GrayscaleConverter {
  public:
    static std::uint8_t * Rec601(BMP::Pixel const* data, int width,
        int height) noexcept;
    static std::uint8_t * Rec709(BMP::Pixel const* data, int width,
        int height) noexcept;
    static std::uint8_t * Mean(BMP::Pixel const* data, int width,
        int height) noexcept;
  };
}

#endif