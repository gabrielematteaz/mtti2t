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
      auto TEMPORARY = Erosion()(binarized_data, width, height);

      return Dilatation()(TEMPORARY.value(), width, height);
    }

    Pointer < std::uint8_t > Closing::operator () (std::uint8_t const *binarized_data, int width, int height) noexcept {
      auto TEMPORARY = Dilatation()(binarized_data, width, height);

      return Erosion()(TEMPORARY.value(), width, height);
    }
  }
}