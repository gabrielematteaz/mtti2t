#ifndef MTTI2T_RESIZER_H_
#define MTTI2T_RESIZER_H_

#include <cstdint>

namespace mtti2t {
  class Resizer {
  public:
    static std::uint8_t *WithoutInterpolation(std::uint8_t * data, int width, int height,
        int width_destination, int height_destination) noexcept;
  };
}

#endif