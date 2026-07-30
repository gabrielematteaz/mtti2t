#include "text_finder.h"

#include <new>

namespace mtti2t {
  std::vector < TextFinder::Rectangle > TextFinder::Histogram(std::uint8_t * data, int width, int height) noexcept {
    int * rows = new(std::nothrow) int[height];
    int * columns = new(std::nothrow) int[width];
    std::vector < TextFinder::Rectangle > rectangles;

    if (rows == nullptr || columns == nullptr) {
      delete[] rows;
      delete[] columns;

      return rectangles;
    }

    for (int index = 0; index < height; ++index) {
      rows[index] = 0;
    }

    for (int index = 0; index < width; ++index) {
      columns[index] = 0;
    }

    int pixel_count = width * height;

    for (int index = 0; index < pixel_count; ++index) {
      int x = index % width;
      int y = index / width;

      columns[x] += data[index];
      rows[y] += data[index];
    }

    try {
      std::vector < TextFinder::Slice > x_slices;
      std::vector < TextFinder::Slice > y_slices;
      int x_slice_start = -1;
      int y_slice_start = -1;

      for (int index = 0; index < height; ++index) {
        if (rows[index] == 0) {
          if (x_slice_start == -1) {
            continue;
          }

          x_slices.emplace_back(x_slice_start, index);
          x_slice_start = -1;
        }
        else if (x_slice_start == -1) {
          x_slice_start = index;
        }
      }

      if (x_slice_start != -1) {
        x_slices.emplace_back(x_slice_start, height);
      }

      for (int index = 0; index < width; ++index) {
        if (columns[index] == 0) {
          if (y_slice_start == -1) {
            continue;
          }

          y_slices.emplace_back(y_slice_start, index);
          y_slice_start = -1;
        }
        else if (y_slice_start == -1) {
          y_slice_start = index;
        }
      }

      if (y_slice_start != -1) {
        y_slices.emplace_back(y_slice_start, width);
      }

      rectangles.reserve(x_slices.size() * y_slices.size());

      for (Slice const& y_slice : y_slices) {
        for (Slice const& x_slice : x_slices) {
          rectangles.emplace_back(y_slice.start, x_slice.start, y_slice.end, x_slice.end);
        }
      }

      return rectangles;
    }
    catch (...) { }

    delete[] rows;
    delete[] columns;

    return rectangles;
  }
}