#ifndef MTTI2T_BMP_H_
#define MTTI2T_BMP_H_

#include <cstdint>

namespace mtti2t {
  class BMP {
  public:
    struct Pixel {
      std::uint8_t r;
      std::uint8_t g;
      std::uint8_t b;
      std::uint8_t unused;
    };
  };
}

#endif