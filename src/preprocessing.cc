#include "preprocessing.h"

#include <cassert>

namespace mtti2t {
  namespace preprocessing {
    Pointer < std::uint8_t > GetMask(HSV::Pixel const* data, int width, int height, Boundaries boundaries) noexcept {
      assert(data != nullptr && width > 0 && height > 0);

      int pixel_count = width * height;
      Pointer < std::uint8_t > mask(pixel_count);

      if (mask) {
        std::uint8_t * mask_raw = mask.value();

        for (int index = 0; index < pixel_count; ++index) {
          HSV::Pixel pixel = data[index];

          bool hue_condition = boundaries.low.hue <= boundaries.high.hue ?
              pixel.hue >= boundaries.low.hue && pixel.hue <= boundaries.high.hue :
              pixel.hue >= boundaries.low.hue || pixel.hue <= boundaries.high.hue;

          if (hue_condition &&
              pixel.saturation >= boundaries.low.saturation && pixel.saturation <= boundaries.high.saturation &&
              pixel.value >= boundaries.low.value && pixel.value <= boundaries.high.value) {
            mask_raw[index] = 255;
          }
          else {
            mask_raw[index] = 0;
          }
        }

        // TODO: erode and dilate
      }

      return mask;
    }
  }
}