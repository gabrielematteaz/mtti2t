#include "text_finder.h"

namespace mtti2t {
  namespace text_finders {
    std::vector < Rectangle > ConnectedComponentsLabeling::operator () (std::uint8_t const* binarized_data, int width, int height) noexcept {
      if (binarized_data == nullptr || width < 0 || height < 0) {
        return { };
      }

      int pixel_count = width * height;
      std::optional < UnionFind > union_find = UnionFind::Construct(pixel_count);

      if (union_find.has_value() == false) {
        return { };
      }

      int x_offset[] = { -1, 0, +1, -1 };
      int y_offset[] = { -1, -1, -1, 0 };

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int offset = y * width + x;

          if (binarized_data[offset] == 0) {
            for (int index = 0; index < 4; ++index) {
              int x_offset_current = x + x_offset[index];
              int y_offset_current = y + y_offset[index];
              int offset_current = y_offset_current * width + x_offset_current;

              if (x_offset_current >= 0 && x_offset_current < width &&
                  y_offset_current >= 0 && y_offset_current < height &&
                  binarized_data[offset_current] == 0) {
                union_find->Union(offset, offset_current);
              }
            }
          }
        }
      }

      Pointer < int > indexes(pixel_count);
      int * indexes_raw = indexes.value();

      if (indexes_raw == nullptr) {
        return { };
      }

      for (int index = 0; index < pixel_count; ++index) {
        indexes_raw[index] = -1;
      }

      std::vector < Rectangle > rectangles;

      for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          int offset = y * width + x;

          if (binarized_data[offset] == 0) {
            int root = union_find->Find(offset);

            if (indexes_raw[root] == -1) {
              indexes_raw[root] = static_cast < int > (rectangles.size());
              rectangles.emplace_back();
            }

            int index = indexes_raw[root];

            rectangles[index].Add(x, y);
          }
        }
      }

      return rectangles;
    }
  }
}