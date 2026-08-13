#ifndef MTTI2T_GRAYSCALE_CONVERTER_H_
#define MTTI2T_GRAYSCALE_CONVERTER_H_

#include <cstdint>
#include <variant>

#include "data_structures/pointer.h"
#include "RGB.h"

namespace mtti2t {
  namespace grayscale_converters {
    class Recommendation601 {
    public:
      Pointer < std::uint8_t > operator () (RGB::Pixel const* data, int width, int height) noexcept;
    };

    class Recommendation709 {
    public:
      Pointer < std::uint8_t > operator () (RGB::Pixel const* data, int width, int height) noexcept;
    };

    class ArithmeticMean {
    public:
      Pointer < std::uint8_t > operator () (RGB::Pixel const* data, int width, int height) noexcept;
    };

    using GrayscaleConverter = std::variant < Recommendation601, Recommendation709, ArithmeticMean >;

    template < typename GrayscaleConverterType >
    inline Pointer < std::uint8_t > ApplyGrayscaleConversion(GrayscaleConverterType & grayscale_converter,
        RGB::Pixel const* data, int width, int height) noexcept {
      return grayscale_converter(data, width, height);
    }

    inline Pointer < std::uint8_t > ApplyGrayscaleConversion(GrayscaleConverter & grayscale_converter,
        RGB::Pixel const* data, int width, int height) noexcept {
      return std::visit([=] (auto & grayscale_converter) noexcept -> Pointer < std::uint8_t > {
        return ApplyGrayscaleConversion(grayscale_converter, data, width, height);
      }, grayscale_converter);
    }
  };
}

#endif