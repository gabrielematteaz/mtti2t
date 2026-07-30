#include "binarizer.h"

#ifdef _DEBUG
#include <iostream>
#endif
#include <new>

namespace mtti2t {
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
    int block_width = width / width_blocks_;
    int block_height = height / height_blocks_;
    int width_remainder = width % width_blocks_;
    int height_remainder = height % height_blocks_;

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int x_block = x / block_width;
      bool x_adjustment = x_block < width_remainder && x % block_width == 0;

      if (x_adjustment) {
        --x_block;
      }

      int y = index / width;
      int y_block = y / block_height;
      bool y_adjustment = y_block < height_remainder &&
          y % block_height == 0;

      if (y_adjustment) {
        --y_block;
      }

#ifdef _DEBUG
      std::cout << index << '\n';
      std::cout << x << ' ' << x_block << ' ' << x_adjustment << '\n';
      std::cout << y << ' ' << y_block << ' ' << y_adjustment << '\n';
#endif

      local_thresholds[y_block * width_blocks_ + x_block] += data[index];
    }

    for (int index = 0; index < block_count; ++index) {
      int real_width = block_width + (index % width_blocks_ <
          width_remainder ? 1 : 0);
      int real_height = block_height + (index / width_blocks_ <
          height_remainder ? 1 : 0);
      int area = real_width * real_height;

      local_thresholds[index] /= area;

#ifdef _DEBUG
      std::cout << real_width << ' ' << real_height <<
          local_thresholds[index] << '\n';
#endif
    }

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int x_block = x / block_width;
      bool x_adjustment = x_block < width_remainder && x % block_width == 0;

      if (x_adjustment) {
        --x_block;
      }

      int y = index / width;
      int y_block = y / block_height;
      bool y_adjustment = y_block < height_remainder &&
          y % block_height == 0;

      if (y_adjustment) {
        --y_block;
      }

      data[index] = data[index] < local_thresholds[y_block * width_blocks_ +
          x_block] ? 0 : 255;
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