#include "noise_filter.h"

namespace mtti2t {
  namespace noise_filters {
    Pointer < double > Gaussian::GetKernel(int size, double sigma) noexcept {
      assert(size > 0);

      int total_size = size * size;
      Pointer < double > kernel(total_size);
      double * kernel_raw = kernel.value();

      if (kernel_raw != nullptr) {
        double sum = 0.0;
        int radius = size / 2;
        double denominator = 2 * sigma * sigma;

        for (int y = -radius; y <= radius; ++y) {
          int y_real = y + radius;

          for (int x = -radius; x <= radius; ++x) {
            double value = std::exp(-(x * x + y * y) / denominator);

            kernel_raw[y_real * size + x + radius] = value;
            sum = sum + value;
          }
        }

        // normalization
        for (int index = 0; index < total_size; ++index) {
          kernel_raw[index] = kernel_raw[index] / sum;
        }
       }

      return kernel;
    }

    Pointer < std::uint8_t > Gaussian::operator () (std::uint8_t const* grayscale_data, int width, int height) noexcept {
      if (grayscale_data == nullptr || width <= 0 || height <= 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > result_data(pixel_count);
      std::uint8_t * result_data_raw = result_data.value();
      Pointer < double > kernel = GetKernel(size_, sigma_);
      double * kernel_raw = kernel.value();

      if (result_data_raw == nullptr || kernel_raw == nullptr) {
        return { };
      }

      int radius = size_ / 2;

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          double value = 0.0;

          for (int y_radius = -radius; y_radius <= radius; ++y_radius) {
            for (int x_radius = -radius; x_radius <= radius; ++x_radius) {
              int x_real = x + x_radius;
              int y_real = y + y_radius;

              if (y_real < 0 || y_real >= height || x_real < 0 || x_real >= width) {
                continue;
              }

              value = value + grayscale_data[y_real * width + x_real] *
                  kernel_raw[(y_radius + radius) * size_ + (x_radius + radius)];
            }
          }

          result_data_raw[y * width + x] = static_cast < std::uint8_t > (value);
        }
      }

      return result_data;
    }

    Pointer < std::uint8_t > Dilatation::operator () (std::uint8_t const* binarized_data, int width, int height) noexcept {
      if (binarized_data == nullptr || width <= 0 || height <= 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > dilated_data(pixel_count);
      std::uint8_t * dilated_data_raw = dilated_data.value();

      if (dilated_data_raw == nullptr) {
        return { };
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int offset = y * width + x;
          int left_offset = offset - 1;
          int up_offset = offset - width;
          int right_offset = offset + 1;
          int down_offset = offset + width;

          dilated_data_raw[offset] = (binarized_data[offset] == 255 &&
              (x <= 0 || binarized_data[left_offset] == 255) &&
              (y <= 0 || binarized_data[up_offset] == 255) &&
              (x >= width - 1 || binarized_data[right_offset] == 255) &&
              (y >= height - 1 || binarized_data[down_offset] == 255)) ? 255 : 0;
        }
      }

      return dilated_data;
    }

    Pointer < std::uint8_t > Erosion::operator () (std::uint8_t const* binarized_data, int width, int height) noexcept {
      if (binarized_data == nullptr || width <= 0 || height <= 0) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > dilated_data(pixel_count);
      std::uint8_t * dilated_data_raw = dilated_data.value();

      if (dilated_data_raw == nullptr) {
        return { };
      }

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int offset = y * width + x;
          int left_offset = offset - 1;
          int up_offset = offset - width;
          int right_offset = offset + 1;
          int down_offset = offset + width;

          dilated_data_raw[offset] = binarized_data[offset] == 255 ||
              (x > 0 && binarized_data[left_offset] == 255) ||
              (y > 0 && binarized_data[up_offset] == 255) ||
              (x < width - 1 && binarized_data[right_offset] == 255) ||
              (y < height - 1 && binarized_data[down_offset] == 255) ? 255 : 0;
        }
      }

      return dilated_data;
    }

    Pointer < std::uint8_t > Opening::operator () (std::uint8_t const *binarized_data, int width, int height) noexcept {
      return { };
    }

    Pointer < std::uint8_t > Closing::operator () (std::uint8_t const *binarized_data, int width, int height) noexcept {
      return { };
    }
  }
}