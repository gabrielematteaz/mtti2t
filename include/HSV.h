#ifndef MTTI2T_HSV_H_
#define MTTI2T_HSV_H_

#include <algorithm>
#include <cstdint>

#include "data_structures/pointer.h"
#include "RGB.h"

namespace mtti2t {
  namespace HSV {
    struct Pixel {
      float hue;
      float saturation;
      float value;
    };

    Pointer < Pixel > ToHSV(RGB::Pixel const* data, int width, int height) noexcept;
  }
}

#endif