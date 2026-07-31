#include "density_calculator.h"

#include <new>

namespace mtti2t {
  std::uint8_t * MeanDensityCalculator::operator () (std::uint8_t * data, int width, int height) noexcept {
    // TODO: more robust checks
    if (width < width_blocks_ || height < height_blocks_) {
      return nullptr;
    }

    int block_count = width_blocks_ * height_blocks_;
    int * densities = new(std::nothrow) int[block_count];

    if (densities == nullptr) {
      return nullptr;
    }

    for (int index = 0; index < block_count; ++index) {
      densities[index] = 0;
    }

    int pixel_count = width * height;
    double block_width = static_cast < double > (width) / width_blocks_;
    double block_height = static_cast < double > (height) / height_blocks_;

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int x_block = static_cast < double > (x) / block_width;
      int y = index / width;
      int y_block = static_cast < double > (y) / block_height;

      densities[y_block * width_blocks_ + x_block] += data[index];
    }

    double area = block_width * block_height;

    for (int index = 0; index < block_count; ++index) {
      densities[index] = static_cast < double > (densities[index]) / area;
    }

    std::uint8_t * real_densities = new(std::nothrow) std::uint8_t[block_count];
    
    if (real_densities != nullptr) {
      for (int index = 0; index < block_count; ++index) {
        real_densities[index] = densities[index];
      }
    }

    delete[] densities;

    return real_densities;
  }
}