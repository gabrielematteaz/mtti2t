#include "grayscale_converter.h"

#include <cmath>
#include <new>

namespace mtti2t {
  // note: lrint is fast but not optimal as it relies on the current rounding mode

  namespace grayscale_converters {
    Pointer < std::uint8_t > Recommendation601::operator () (RGB const* data, int width, int height) noexcept {
      if (data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > grayscale_data(pixel_count);
      std::uint8_t *grayscale_data_raw = grayscale_data.value();

      if (grayscale_data_raw != nullptr) {
        for (int index = 0; index < pixel_count; ++index) {
          grayscale_data_raw[index] = std::lrint(0.299 * data[index].r + 0.587 * data[index].g + 0.114 * data[index].b);
        }
      }

      return grayscale_data;
    }

    Pointer < std::uint8_t > Recommendation709::operator () (RGB const* data, int width, int height) noexcept {
      if (data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > grayscale_data(pixel_count);
      std::uint8_t *grayscale_data_raw = grayscale_data.value();

      if (grayscale_data_raw != nullptr) {
        for (int index = 0; index < pixel_count; ++index) {
          grayscale_data_raw[index] = std::lrint(0.2126 * data[index].r + 0.7152 * data[index].g + 0.0722 * data[index].b);
        }
      }

      return grayscale_data;
    }

    Pointer < std::uint8_t > ArithmeticMean::operator () (RGB const* data, int width, int height) noexcept {
      if (data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > grayscale_data(pixel_count);
      std::uint8_t *grayscale_data_raw = grayscale_data.value();

      if (grayscale_data_raw != nullptr) {
        for (int index = 0; index < pixel_count; ++index) {
          grayscale_data_raw[index] = std::lrint(static_cast < double > (data[index].r + data[index].g + data[index].b) / 3);
        }
      }

      return grayscale_data;
    }
  };
}