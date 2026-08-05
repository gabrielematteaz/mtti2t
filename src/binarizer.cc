#include "binarizer.h"

#include <algorithm>
#include <cmath>
#include <new>
#include <utility>

// #include <iostream>

namespace mtti2t {
  // rewrite niblack and sauvola better + add optimization flag (size vs. speed)
  // figure out why optimized version is different than not optimized one
  namespace binarizers {
    Pointer < std::uint8_t >  SauvolaThreshold::operator () (std::uint8_t * grayscale_data, int width, int height) noexcept {
      if (grayscale_data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > binary_data(pixel_count);
      std::uint8_t * binary_data_raw = binary_data.value();

      if (binary_data_raw == nullptr) {
        return { };
      }

      bool result;

      if (use_integral_images_) {
        result = WithIntegralImages(grayscale_data, binary_data_raw, width, height);
      }
      else {
        result = WithoutIntegralImages(grayscale_data, binary_data_raw, width, height);
      }

      if (result == false) {
        return { };
      }

      return binary_data;
    }

    bool SauvolaThreshold::WithIntegralImages(std::uint8_t * grayscale_data, std::uint8_t * binary_data, int width,
          int height) noexcept {
      Pointer < Integrals > integral_image(width * height);
      Integrals * integral_image_raw = integral_image.value();

      if (integral_image_raw == nullptr) {
        return false;
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int offset = y * width + x;
          std::uint64_t val = grayscale_data[offset];
          
          std::uint64_t left_sum = (x > 0) ? integral_image_raw[offset - 1].sum : 0;
          std::uint64_t up_sum = (y > 0) ? integral_image_raw[offset - width].sum : 0;
          std::uint64_t up_left_sum = (x > 0 && y > 0) ? integral_image_raw[offset - width - 1].sum : 0;

          std::uint64_t left_sum_squared = (x > 0) ? integral_image_raw[offset - 1].sum_squared : 0;
          std::uint64_t up_sum_squared = (y > 0) ? integral_image_raw[offset - width].sum_squared : 0;
          std::uint64_t up_left_sum_squared = (x > 0 && y > 0) ? integral_image_raw[offset - width - 1].sum_squared : 0;

          integral_image_raw[offset].sum = val + left_sum + up_sum - up_left_sum;
          integral_image_raw[offset].sum_squared = (val * val) + left_sum_squared + up_sum_squared - up_left_sum_squared;
        }
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int x1 = std::max(0, x - width_radius_);
          int y1 = std::max(0, y - height_radius_);
          int x2 = std::min(width - 1, x + width_radius_);
          int y2 = std::min(height - 1, y + height_radius_);
          int area = (x2 - x1 + 1) * (y2 - y1 + 1);

          auto get_sum = [&](int x, int y) {
            if (x < 0 || y < 0) {
              return Integrals{ 0, 0 };
            }

            return integral_image_raw[y * width + x];
          };

          Integrals bottom_right = get_sum(x2, y2);
          Integrals bottom_left = get_sum(x1 - 1, y2);
          Integrals top_right = get_sum(x2, y1 - 1);
          Integrals top_left = get_sum(x1 - 1, y1 - 1);
          std::uint64_t sum = bottom_right.sum - bottom_left.sum - top_right.sum + top_left.sum;
          std::uint64_t sum_squared = bottom_right.sum_squared - bottom_left.sum_squared -
              top_right.sum_squared + top_left.sum_squared;

          double mean = static_cast < double > (sum) / area;
          double variance = static_cast < double > (sum_squared) / area - (mean * mean);
          double standard_deviation = std::sqrt(std::max(0.0, variance));
          double threshold = mean * (1.0 + K_ * (standard_deviation / 128.0 - 1.0));

          int offset = y * width + x;

          binary_data[offset] = grayscale_data[offset] < threshold ? 0 : 255;
        }
      }

      return true;
    }

    bool SauvolaThreshold::WithoutIntegralImages(std::uint8_t * grayscale_data, std::uint8_t * binary_data, int width,
          int height) noexcept {
      for (int y = 0, offset = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x, ++offset) {
          double sum = 0;
          double squared_sum = 0;
          int count = 0;

          for (int y_window = y - height_radius_; y_window <= y + height_radius_; ++y_window) {
            for (int x_window = x - width_radius_; x_window <= x + width_radius_; ++x_window) {
              if (y_window >= 0 && y_window < height && x_window >= 0 && x_window < width) {
                double value = static_cast < double > (grayscale_data[y_window * width + x_window]);

                sum = sum + value;
                squared_sum = squared_sum + value * value;
                ++count;
              }
            }
          }

          double mean = sum / count;
          double variance = (squared_sum / count) - (mean * mean);
          double standard_deviation = std::sqrt(std::max(0.0, variance));

          double threshold = mean * (1.0 + K_ * (standard_deviation / 128.0 - 1.0));

          binary_data[offset] = grayscale_data[offset] < threshold ? 0 : 255;
        }
      }

      return true;
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