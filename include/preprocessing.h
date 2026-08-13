#ifndef MTTI2T_PREPROCESSING_H_
#define MTTI2T_PREPROCESSING_H_

#include <cstdint>

#include "data_structures/pointer.h"
#include "HSV.h"
#include "RGB.h"

namespace mtti2t {
  namespace preprocessing {
    struct Boundaries {
      HSV::Pixel low;
      HSV::Pixel high;
    };

    constexpr Boundaries GetDarkTextBoundaries() noexcept {
      return { 0.0f, 0.0f, 0.0f, 360.0f, 1.0f, 0.3f };
    }

    constexpr Boundaries GetLightTextBoundaries() noexcept {
      return { 0.0f, 0.0f, 0.7f, 360.0f, 0.2f, 1.0f };
    }

    Pointer < std::uint8_t > GetMask(HSV::Pixel const* data, int width, int height, Boundaries boundaries) noexcept;
  }
}

#endif