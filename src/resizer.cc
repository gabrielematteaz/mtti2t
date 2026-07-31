#include "resizer.h"

#include <cstddef>
#include <new>

namespace mtti2t {
  std::uint8_t * Resizer::Sample(std::uint8_t * data, int width, int height,
      int width_destination, int height_destination) noexcept {
    std::uint8_t * resized_data = new(std::nothrow) std::uint8_t[width_destination * height_destination];

    if (resized_data == nullptr) {
      return nullptr;
    }

    double width_step = static_cast < double > (width) / width_destination;
    double height_step = static_cast < double > (height) / height_destination;
    int destination_offset = 0;

    for (double height_offset = 0; height_offset < height; height_offset += height_step) {
      for (double width_offset = 0; width_offset < width; width_offset += width_step) {
        std::size_t truncated_width = width_offset;
        std::size_t truncated_height = height_offset;

        resized_data[destination_offset] = data[truncated_height * width + truncated_width];
        ++destination_offset;
      }
    }

    return resized_data;
  }
}