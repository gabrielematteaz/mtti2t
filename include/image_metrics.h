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

      double sum = 0.0;

      for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
          double value = 0.0;

          // apply immerkaer kernel
          value = value + grayscale_data[(y - 1) * width + (x - 1)];
          value = value - 2.0 * grayscale_data[(y - 1) * width + x];
          value = value + grayscale_data[(y - 1) * width + (x + 1)];
          value = value - 2.0 * grayscale_data[y * width + (x - 1)];
          value = value + 4.0 * grayscale_data[y * width + x];
          value = value - 2.0 * grayscale_data[y * width + (x + 1)];
          value = value + grayscale_data[(y + 1) * width + (x - 1)];
          value = value - 2.0 * grayscale_data[(y + 1) * width + x];
          value = value + grayscale_data[(y + 1) * width + (x + 1)];

          sum = sum + std::abs(value);
        }
      }

      double K = 1.25331413732; // SQRT(PI/2)
      double M = 6.0; // standard deviation of kernel

      return sum * K / (M * (width - 2) * (height - 2));
    }

    inline double GetEntropy(std::uint8_t const* grayscale_data, int width, int height) noexcept {
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

    // text documents usually have this around 0.03 and 0.10
    inline double GetEdgeDensity(std::uint8_t const* grayscale_data, int width, int height) noexcept {
      assert(grayscale_data != nullptr && width > 0 && height > 0);

      // Sobel kernels
      int x_kernels[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
      int y_kernels[3][3] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

      double total_magnitude = 0.0;

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          double gradient_1 = 0;
          double gradient_2 = 0;

          for (int I = -1; I <= 1; ++I) {
            int y_real = y + I;

            for (int J = -1; J <= 1; ++J) {
              int x_real = x + J;

              if (y_real < 0 || y_real >= height || x_real < 0 || x_real >= width) {
                continue;
              }

              std::uint8_t pixel = grayscale_data[y_real * width + x_real];

              gradient_1 = gradient_1 + pixel * x_kernels[I + 1][J + 1];
              gradient_2 = gradient_2 + pixel * y_kernels[I + 1][J + 1];
            }
          }

          // use manhattan distance if need to be fast -> normalization costant must be width * height * 2040
          double magnitude = std::sqrt(gradient_1 * gradient_1 + gradient_2 * gradient_2);

          total_magnitude = total_magnitude + magnitude;
        }
      }

      // SQRT((4*255)^2 + (4*255)^2) is circa 1141
      double maximum = width * height * 1141.0;

      return static_cast < double > (total_magnitude) / maximum;
    }
  }
}

#endif