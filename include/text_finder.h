#ifndef MTTI2T_TEXT_FINDER_H_
#define MTTI2T_TEXT_FINDER_H_

#include <cstdint>
#include <vector>

namespace mtti2t {
  class TextFinder {
  public:
    struct Slice {
      int start;
      int end;
    };

    struct Rectangle {
      int x;
      int y;
      int x2;
      int y2;
    };

    static std::vector < Rectangle > Histogram(std::uint8_t * data, int width, int height) noexcept;
  };
}

#endif