#include "HSV.h"

#include <cassert>

namespace mtti2t {
  namespace HSV {
    Pointer < Pixel > ToHSV(RGB::Pixel const* data, int width, int height) noexcept {
      assert(data != nullptr && width > 0 && height > 0);

      int pixel_count = width * height;
      Pointer < Pixel > HSV_data(pixel_count);

      if (HSV_data) {
        Pixel * HSV_data_raw = HSV_data.value();

        for (int index = 0; index < pixel_count; ++index) {
          constexpr float inverse_255 = 1.0f / 255.0f;

          float values[3] = {
            data[index].r * inverse_255, // r
            data[index].g * inverse_255, // g
            data[index].b * inverse_255, // b
          };

          auto [minimum, maximum] = std::ranges::minmax(values);
          float delta = maximum - minimum;
          float hue = 0.0f;

          if (delta != 0.0f) {
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
          }

          HSV_data_raw[index].hue = hue;
          HSV_data_raw[index].saturation = maximum <= 0.0f ? 0.0f : delta / maximum;
          HSV_data_raw[index].value = maximum;
        }
      }

      return HSV_data;
    }
  }
}