#ifndef MTTI2T_STATISTICS_H_
#define MTTI2T_STATISTICS_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>

namespace mtti2t {
  namespace image_metrics {
    struct Basics {
      double brightness;
      double contrast;
      double CV; // coefficient of variation
    };

    inline Basics GetBasics(std::uint8_t const* grayscale_data, int width, int height) noexcept {
      assert(grayscale_data != nullptr && width > 0 && height > 0);

      int pixel_count = width * height;
      std::int64_t sum = 0;
      std::int64_t squared_sum = 0;

      for (int index = 0; index < pixel_count; ++index) {
        std::uint8_t pixel = grayscale_data[index];

        sum = sum + pixel;
        squared_sum = squared_sum + pixel * pixel;
      }

      double arithmetic_mean = static_cast < double > (sum) / pixel_count;
      double variance = static_cast < double > (squared_sum) / pixel_count - arithmetic_mean * arithmetic_mean;
      double standard_deviation = std::sqrt(std::max(0.0, variance));

      return { arithmetic_mean, standard_deviation,
          std::abs(arithmetic_mean) > 1e-9 ? standard_deviation / arithmetic_mean : 0.0 };
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

    inline double GetBimodality(std::uint8_t const* grayscale_data, int width, int height) noexcept {
      assert(grayscale_data != nullptr && width > 0 && height > 0);

      int intensities[256]{ };
      int pixel_count = width * height;

      for (int index = 0; index < pixel_count; ++index) {
        ++intensities[grayscale_data[index]];
      }

      std::int64_t total_intensity = 0;

      for (int index = 0; index < 256; ++index) {
        total_intensity = total_intensity + index * intensities[index];
      }

      double arithmetic_mean = static_cast < double > (total_intensity) / pixel_count;
      double second_moment = 0.0;
      double third_moment = 0.0;
      double fourth_moment = 0.0;

      for (int index = 0; index < 256; ++index) {
        if (intensities[index] == 0) {
          continue;
        }

        double difference = index - arithmetic_mean;
        double squared_difference = difference * difference;

        second_moment = second_moment + (squared_difference * intensities[index]);
        third_moment = third_moment + (squared_difference * difference * intensities[index]);
        fourth_moment = fourth_moment + (squared_difference * squared_difference * intensities[index]);
      }

      second_moment = second_moment / pixel_count;
      third_moment = third_moment / pixel_count;
      fourth_moment = fourth_moment / pixel_count;

      if (second_moment < 1e-9) { // unimodal
        return 0.0;
      }

      double skewness = third_moment / std::pow(second_moment, 1.5);
      double kurtosis = fourth_moment / (second_moment * second_moment); // Pearson's kurtosis

      return (skewness * skewness + 1) / kurtosis;
    }
  }
}

#endif