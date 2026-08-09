#ifndef MTTI2T_TEXT_FINDER_H_
#define MTTI2T_TEXT_FINDER_H_

#include <cstdint>
#include <vector> // TEMP

#include "data_structures/pointer.h"
#include "data_structures/union_find.h"

namespace mtti2t {
  namespace text_finders {
    // prova con MSER

    // maybe move internal of CCL
    struct Rectangle {
      int x_min = 0x8fffffff;
      int y_min = 0x8fffffff;
      int x_max = -1;
      int y_max = -1;
      int pixel_count = 0;

      void Add(int x, int y) noexcept {
        if (x < x_min) {
          x_min = x;
        }

        if (x > x_max) {
          x_max = x;
        }

        if (y < y_min) {
          y_min = y;
        }

        if (y > y_max) {
          y_max = y;
        }

        ++pixel_count;
      }
    };

    class ConnectedComponentsLabeling {
    public:
      std::vector < Rectangle > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };
  }
}

#endif