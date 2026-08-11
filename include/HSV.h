#ifndef MTTI2T_HSV_H_
#define MTTI2T_HSV_H_

#include <algorithm>
#include <cstdint>

#include "data_structures/pointer.h"
#include "RGB.h"

namespace mtti2t {
  struct HSV {
    float hue;
    float saturation;
    float value;
  };

  struct Boundaries {
    HSV low;
    HSV high;
  };

  Pointer < HSV > ToHSV(RGB const* data, int width, int height) noexcept;
  Pointer < std::uint8_t > GetMask(HSV const* HSV_data, int width, int height, Boundaries boundaries) noexcept;
  Pointer < RGB > ApplyMask(RGB const* RGB_data, std::uint8_t const* mask_data, int width, int height) noexcept;

  inline Boundaries GetDarkTextBoundaries() noexcept {
    return { 0.0f, 0.0f, 0.0f, 360.0f, 1.0f, 0.3f };
  }

  inline Boundaries GetLightTextBoundaries() noexcept {
    return { 0.0f, 0.0f, 0.7f, 360.0f, 0.2f, 1.0f };
  }
}

#endif