#ifndef MTTI2T_RGB_H_
#define MTTI2T_RGB_H_

#include <cstdint>

namespace mtti2t {
  namespace RGB {
    struct Pixel {
      std::uint8_t r;
      std::uint8_t g;
      std::uint8_t b;
    };
  }
}

#endif