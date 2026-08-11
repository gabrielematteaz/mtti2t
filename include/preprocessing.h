#ifndef MTTI2T_PREPROCESSING_H_
#define MTTI2T_PREPROCESSING_H_

#include <cstdint>

#include "data_structures/pointer.h"
#include "HSV.h"

namespace mtti2t {
  inline Pointer < std::uint8_t > MergeMask(std::uint8_t const* mask_1, std::uint8_t const* mask_2, int width, int height) noexcept {
    if (mask_1 == nullptr || mask_2 == nullptr || width <= 0 || height <= 0) {
      return { };
    }

    int pixel_count = width * height;
    Pointer < std::uint8_t > result(pixel_count);
    std::uint8_t * result_raw = result.value();

    if (result_raw == nullptr) {
      return { };
    }

    for (int index = 0; index < pixel_count; ++index) {
      result_raw[index] = mask_1[index] | mask_2[index];
    }

    return result;
  }

  inline void Invert(std::uint8_t * mask, int width, int height) noexcept {
    if (mask == nullptr || width <= 0 || height <= 0) {
      return;
    }

    for (int index = 0, pixel_count = width * height; index <= pixel_count; ++index) {
      mask[index] = 255 - mask[index];
    }
  }
}

#endif