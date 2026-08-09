#ifndef MTTI2T_STATISTICS_H_
#define MTTI2T_STATISTICS_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <utility>

namespace mtti2t {
  namespace image_metrics {
    inline std::pair < double, double > GetBrightnessAndContrast(std::uint8_t const* grayscale_data, int width,
        int height) noexcept {
      assert(grayscale_data != nullptr && width > 0 && height > 0);

      int pixel_count = width * height;
      double sum = 0;
      double squared_sum = 0;

      for (int index = 0; index < pixel_count; ++index) {
        sum = sum + grayscale_data[index];
        squared_sum = squared_sum + grayscale_data[index] * grayscale_data[index];
      }

      std::pair < double, double > result;

      double arithmetic_mean = sum / pixel_count;
      double variance = squared_sum / pixel_count - arithmetic_mean * arithmetic_mean;

      return { arithmetic_mean, std::sqrt(std::max(0.0, variance)) };
    }

    inline double GetNoisiness(std::uint8_t const* grayscale_data, int width, int height) noexcept {
      assert(grayscale_data != nullptr && width > 0 && height > 0);

      int frequencies[256]{ };
      int pixel_count = width * height;

      for (int index = 0; index < pixel_count; ++index) {
        ++frequencies[grayscale_data[index]];
      }

      double sum = 0;

      for (int frequency : frequencies) {
        if (frequency != 0) {
          double probability = static_cast < double > (frequency) / pixel_count;

          sum = sum + probability * std::log2(probability);
        }
      }

      return -sum;
    }
  }
}

#endif