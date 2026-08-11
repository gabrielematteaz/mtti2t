#include "HSV.h"

#include "noise_filter.h"

namespace mtti2t {
  Pointer < HSV > ToHSV(RGB const* data, int width, int height) noexcept {
    if (data == nullptr || width <= 0 || height <= 0) {
      return { };
    }

    int pixel_count = width * height;
    Pointer < HSV > HSV_data(pixel_count);
    HSV * HSV_data_raw = HSV_data.value();

    if (HSV_data_raw == nullptr) {
      return { };
    }

    for (int index = 0; index < pixel_count; ++index) {
      float inverse_255 = 1.0f / 255.0f;

      float values[3] = {
        data[index].r * inverse_255, // r
        data[index].g * inverse_255, // g
        data[index].b * inverse_255, // b
      };

      auto [minimum, maximum] = std::ranges::minmax(values);
      float delta = maximum - minimum;
      float hue = 0.0f;

      if (delta == 0.0f) {
        hue = 0.0f;
      }
      if (maximum == values[0]) {
        hue = 60.0f * (values[1] - values[2]) / delta;
      }
      else if (maximum == values[1]) {
        hue = 60.0f * ((values[2] - values[0]) / delta + 2);
      }
      else if (maximum == values[2]) {
        hue = 60.0f * ((values[0] - values[1]) / delta + 4);
      }

      if (hue < 0.0f) { // cannot be negative
        hue = hue + 360.0f;
      }

      HSV_data_raw[index].hue = hue;
      HSV_data_raw[index].saturation = maximum <= 0.0f ? 0.0f : delta / maximum;
      HSV_data_raw[index].value = maximum;
    }

    return HSV_data;
  }

  Pointer < std::uint8_t > GetMask(HSV const* HSV_data, int width, int height, Boundaries boundaries) noexcept {
    if (HSV_data == nullptr || width <= 0 || height <= 0) {
      return { };
    }

    int pixel_count = width * height;
    Pointer < std::uint8_t > mask(pixel_count);
    std::uint8_t * mask_raw = mask.value();

    if (mask_raw == nullptr) {
      return { };
    }

    for (int index = 0; index < pixel_count; ++index) {
      HSV pixel = HSV_data[index];

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

    mtti2t::noise_filters::Erosion ero;
    mtti2t::noise_filters::Dilatation dil;

    auto TEMP = ero(mask_raw, width, height);
    auto RES = dil(TEMP.value(), width, height);

    return RES;
  }

  Pointer < RGB > ApplyMask(RGB const* RGB_data, std::uint8_t const* mask_data, int width, int height) noexcept {
    if (RGB_data == nullptr || mask_data == nullptr || width <= 0 || height <= 0) {
      return { };
    }

    int pixel_count = width * height;
    Pointer < RGB > data(pixel_count);
    RGB * data_raw = data.value();

    if (data_raw == nullptr) {
      return { };
    }

    RGB black = { 0, 0, 0 };

    for (int index = 0; index < pixel_count; ++index) {
      data_raw[index] = mask_data[index] ? RGB_data[index] : black;
    }

    return data;
  }
}