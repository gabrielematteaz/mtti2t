#include "noise_filter.h"

#include <algorithm>
#include <cmath>
#include <new>

namespace mtti2t {
  namespace noise_filters {
    // maybe also implement Huang if memory is limited / very fast for small radii (< 10)
    Pointer < std::uint8_t > Median::operator () (std::uint8_t const* data, int width, int height) noexcept {
      assert(data != nullptr && width > 0 && height > 0);

      Pointer < Histogram > histograms(width);

      if (!histograms) {
        return { };
      }

      int pixel_count = width * height;
      Pointer < std::uint8_t > filtered_data(pixel_count);

      if (filtered_data) {
        Histogram * histograms_raw = histograms.value();
        std::uint8_t * filtered_data_raw = filtered_data.value();
        int radius = size_ / 2;
        int last_row = height - 1;
        int last_column = width - 1;

        for (int I = 0; I < width; ++I) {
          for (int J = -radius; J <= radius; ++J) {
            int row = std::clamp(J, 0, last_row);

            histograms_raw[I].Add(data[row * width + I]);
          }
        }

        for (int I = 0; I < height; ++I) {
          Histogram histogram;

          for (int J = -radius; J <= radius; ++J) {
            int column = std::clamp(J, 0, last_column);

            histogram.Add(histograms_raw[column]);
          }

          for (int J = 0; J < width; ++J) {
            filtered_data_raw[I * width + J] = histogram.GetMedian();

            int column_add = std::clamp(J + radius + 1, 0, last_column);
            int column_remove = std::clamp(J - radius, 0, last_column);

            histogram.Add(histograms_raw[column_add]);
            histogram.Remove(histograms_raw[column_remove]);
          }

          if (I + 1 < height) {
            for (int J = 0; J < width; ++J) {
              int row_remove = std::clamp(I - radius, 0, last_row);
              int row_add = std::clamp(I + radius + 1, 0, last_row);

              histograms_raw[J].Remove(data[row_remove * width + J]);
              histograms_raw[J].Add(data[row_add * width + J]);
            }
          }
        }
      }

      return filtered_data;
    }

    Pointer < double > Gaussian::GetKernel(int size, double sigma) noexcept {
      assert(size > 0);

      int total_size = size * size;
      Pointer < double > kernel(total_size);

      if (kernel) {
        double * kernel_raw = kernel.value();

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

    Pointer < std::uint8_t > Gaussian::operator () (std::uint8_t const* data, int width, int height) noexcept {
      if (data == nullptr || width <= 0 || height <= 0) {
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

              value = value + data[y_real * width + x_real] *
                  kernel_raw[(y_radius + radius) * size_ + (x_radius + radius)];
            }
          }

          result_data_raw[y * width + x] = static_cast < std::uint8_t > (value);
        }
      }

      return result_data;
    }

    Pointer < std::uint8_t > Dilation::operator () (std::uint8_t const* data, int width, int height) noexcept {
      if (data == nullptr || width <= 0 || height <= 0) {
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

          dilated_data_raw[offset] = (data[offset] == 255 &&
              (x <= 0 || data[left_offset] == 255) &&
              (y <= 0 || data[up_offset] == 255) &&
              (x >= width - 1 || data[right_offset] == 255) &&
              (y >= height - 1 || data[down_offset] == 255)) ? 255 : 0;
        }
      }

      return dilated_data;
    }

    Pointer < std::uint8_t > Erosion::operator () (std::uint8_t const* data, int width, int height) noexcept {
      if (data == nullptr || width <= 0 || height <= 0) {
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

          dilated_data_raw[offset] = data[offset] == 255 ||
              (x > 0 && data[left_offset] == 255) ||
              (y > 0 && data[up_offset] == 255) ||
              (x < width - 1 && data[right_offset] == 255) ||
              (y < height - 1 && data[down_offset] == 255) ? 255 : 0;
        }
      }

      return dilated_data;
    }

    Pointer < std::uint8_t > Opening::operator () (std::uint8_t const *data, int width, int height) noexcept {
      return { };
    }

    Pointer < std::uint8_t > Closing::operator () (std::uint8_t const *data, int width, int height) noexcept {
      return { };
    }
  }
}