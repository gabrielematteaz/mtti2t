#include "binarizer.h"

#include <algorithm>
#include <cmath>
#include <new>

namespace mtti2t {
  namespace binarizers {
    Pointer < std::uint8_t > NiblackThreshold::operator () (std::uint8_t * grayscale_data, int width, int height) noexcept {
      if (grayscale_data == nullptr || width < 0 || height < 0) {
        return { };
      }

      Pointer < std::uint8_t > binary_data(width * height);
      std::uint8_t * binary_data_raw = binary_data.value();

      if (binary_data_raw == nullptr) {
        return { };
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          double sum = 0;
          double squared_sum = 0;
          int count = 0;

          for (int y_window = y - window_height_; y_window <= y + window_height_; ++y_window) {
            for (int x_window = x - window_width_; x_window <= x + window_width_; ++x_window) {
              if (y_window >= 0 && y_window < height && x_window >= 0 && x_window < width) {
                double value = static_cast < double > (grayscale_data[y_window * width + x_window]);

                sum += value;
                squared_sum += value * value;
                count++;
              }
            }
          }

          double mean = sum / count;
          double variance = (squared_sum / count) - (mean * mean);
          double standard_deviation = std::sqrt(std::max(0.0, variance));

          double threshold = mean + K_ * standard_deviation;

          int offset = y * width + x;
          binary_data_raw[offset] = grayscale_data[offset] < threshold ? 0 : 255;
        }
      }

      return binary_data;
    }

    Pointer < std::uint8_t >  SauvolaThreshold::operator () (std::uint8_t * grayscale_data, int width, int height) noexcept {
      if (grayscale_data == nullptr || width < 0 || height < 0) {
        return { };
      }

      Pointer < std::uint8_t > binary_data(width * height);
      std::uint8_t * binary_data_raw = binary_data.value();

      if (binary_data_raw == nullptr) {
        return { };
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          double sum = 0;
          double squared_sum = 0;
          int count = 0;

          for (int y_window = y - window_height_; y_window <= y + window_height_; ++y_window) {
            for (int x_window = x - window_width_; x_window <= x + window_width_; ++x_window) {
              if (y_window >= 0 && y_window < height && x_window >= 0 && x_window < width) {
                double value = static_cast < double > (grayscale_data[y_window * width + x_window]);

                sum += value;
                squared_sum += value * value;
                count++;
              }
            }
          }

          double mean = sum / count;
          double variance = (squared_sum / count) - (mean * mean);
          double standard_deviation = std::sqrt(std::max(0.0, variance));

          constexpr double R = 128.0; 
          double threshold = mean * (1.0 + K_ * (standard_deviation / R - 1.0));

          int offset = y * width + x;
          binary_data_raw[offset] = grayscale_data[offset] < threshold ? 0 : 255;
        }
      }

      return binary_data;
    }

    Pointer < std::uint8_t > GlobalThreshold::operator () (std::uint8_t * grayscale_data, int width, int height) noexcept {
      if (grayscale_data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > binary_data(pixel_count);
      std::uint8_t * binary_data_raw = binary_data.value();

      if (binary_data_raw == nullptr) {
        return { };
      }

      for (int index = 0; index < pixel_count; ++index) {
        binary_data_raw[index] = grayscale_data[index] < threshold_ ? 0 : 255;
      }

      return binary_data;
    }
  };
}