#ifndef MTTI2T_GRAYSCALE_CONVERTER_H_
#define MTTI2T_GRAYSCALE_CONVERTER_H_

#include <cstdint>

#include "data_structures/pointer.h"
#include "RGB.h"

namespace mtti2t {
  namespace grayscale_converters {
    class Recommendation601 {
    public:
      Pointer < std::uint8_t > operator () (RGB const* data, int width, int height) noexcept;
    };

    class Recommendation709 {
    public:
      Pointer < std::uint8_t > operator () (RGB const* data, int width, int height) noexcept;
    };

    class ArithmeticMean {
    public:
      Pointer < std::uint8_t > operator () (RGB const* data, int width, int height) noexcept;
    };
  };
}

#endif