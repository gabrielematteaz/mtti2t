#include "grayscale_converter.h"

#include <new>

namespace mtti2t {
  std::uint8_t * GrayscaleConverter::Rec601(BMP::Pixel const* data,
      int width, int height) noexcept {
    int pixel_count = width * height;
    std::uint8_t * grayscale_data = new(std::nothrow)
        std::uint8_t[pixel_count];

    if (grayscale_data == nullptr) {
      return nullptr;
    }

    for (int index = 0; index < pixel_count; ++index) {
      grayscale_data[index] = 0.299 * data[index].r +
          0.587 * data[index].g + 0.114 * data[index].b;
    }

    return grayscale_data;
  }

  std::uint8_t * GrayscaleConverter::Rec709(BMP::Pixel const* data,
      int width, int height) noexcept {
    int pixel_count = width * height;
    std::uint8_t * grayscale_data = new(std::nothrow)
        std::uint8_t[pixel_count];

    if (grayscale_data == nullptr) {
      return nullptr;
    }

    for (int index = 0; index < pixel_count; ++index) {
      grayscale_data[index] = 0.2126 * data[index].r +
          0.7152 * data[index].g + 0.0722 * data[index].b;
    }

    return grayscale_data;
  }

  std::uint8_t * GrayscaleConverter::Mean(BMP::Pixel const* data,
      int width, int height) noexcept {
    int pixel_count = width * height;
    std::uint8_t * grayscale_data = new(std::nothrow)
        std::uint8_t[pixel_count];

    if (grayscale_data == nullptr) {
      return nullptr;
    }

    for (int index = 0; index < pixel_count; ++index) {
      grayscale_data[index] = (data[index].r + data[index].g +
          data[index].b) / 3;
    }

    return grayscale_data;
  }
}