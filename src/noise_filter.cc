#include "noise_filter.h"

namespace mtti2t {
  namespace noise_filters {
    Pointer < std::uint8_t > Dilatation::operator () (std::uint8_t const* binarized_data, int width, int height) noexcept {
      if (binarized_data == nullptr || width < 0 || height < 0) {
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
          bool found_white = false;

          for (int y_kernel = y - kernel_height_, y_kernel_end = y + kernel_height_;
              !found_white && y_kernel <= y_kernel_end; ++y_kernel) {
            if (y_kernel < 0 || y_kernel >= height) {
              continue;
            }

            for (int x_kernel = x - kernel_width_, x_kernel_end = x + kernel_width_;
                !found_white && x_kernel <= x_kernel_end; ++x_kernel) {
              if (x_kernel < 0 || x_kernel >= width) {
                continue;
              }

              if (binarized_data[y_kernel * width + x_kernel] == 255) {
                found_white = true;
              }
            }
          }

          dilated_data_raw[y * width + x] = found_white ? 255 : 0;
        }
      }

      return dilated_data;
    }

    Pointer < std::uint8_t > Erosion::operator () (std::uint8_t const* binarized_data, int width, int height) noexcept {
      if (binarized_data == nullptr || width < 0 || height < 0) {
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
          bool true_white = true;

          for (int y_kernel = y - kernel_height_, y_kernel_end = y + kernel_height_;
              true_white && y_kernel <= y_kernel_end; ++y_kernel) {
            if (y_kernel < 0 || y_kernel >= height) {
              continue;
            }

            for (int x_kernel = x - kernel_width_, x_kernel_end = x + kernel_width_;
                true_white && x_kernel <= x_kernel_end; ++x_kernel) {
              if (x_kernel < 0 || x_kernel >= width) {
                continue;
              }

              if (binarized_data[y_kernel * width + x_kernel] == 0) {
                true_white = false;
              }
            }
          }

          dilated_data_raw[y * width + x] = true_white ? 255 : 0;
        }
      }

      return dilated_data;
    }
  }
}