#include "binarizer.h"

#include <cmath>
#include <new>

namespace mtti2t {
  // TODO: loops may be faster
  bool MeanBinarizer::operator () (std::uint8_t * data, int width,
      int height) noexcept {
    // TODO: more robust checks
    if (width < width_blocks_ || height < height_blocks_) {
      return false;
    }

    int block_count = width_blocks_ * height_blocks_;
    int * local_thresholds = new(std::nothrow)
        int[block_count];

    if (local_thresholds == nullptr) {
      return false;
    }

    for (int index = 0; index < block_count; ++index) {
      local_thresholds[index] = 0;
    }

    int pixel_count = width * height;
    double block_width = static_cast < double > (width) / width_blocks_;
    double block_height = static_cast < double > (height) / height_blocks_;

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int x_block = static_cast < double > (x) / block_width;
      int y = index / width;
      int y_block = static_cast < double > (y) / block_height;

      local_thresholds[y_block * width_blocks_ + x_block] += data[index];
    }

    double area = block_width * block_height;

    for (int index = 0; index < block_count; ++index) {
      local_thresholds[index] = std::round(static_cast < double > (local_thresholds[index]) / area);
    }

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int x_block = static_cast < double > (x) / block_width;
      int y = index / width;
      int y_block = static_cast < double > (y) / block_height;

      data[index] = data[index] < local_thresholds[y_block * width_blocks_ + x_block] ? 0 : 255;
    }

    delete[] local_thresholds;

    return true;
  }

  bool ThresholdBinarizer::operator () (std::uint8_t * data, int width,
      int height) noexcept {
    for (int index = 0, pixel_count = width * height; index < pixel_count;
        ++index) {
      data[index] = data[index] < threshold_ ? 0 : 255;
    }

    return true;
  }
}