#include <cstdint>
#include <iostream>

#include "RGB.h"
#include "grayscale_converter.h"

int main() {
  mtti2t::RGB array[] = { 255,255,255, 0,0,0, 255,255,255, 0,0,0 };
  mtti2t::grayscale_converters::Recommendation601 grayscale_converter;
  mtti2t::Pointer < std::uint8_t > grayscale_data = grayscale_converter(array, 2, 2);
  std::uint8_t *grayscale_data_raw = grayscale_data.value();

  if (grayscale_data_raw != nullptr) {
    std::cout <<
        static_cast < int > (array[0].r) << ' ' << static_cast < int > (array[0].g) << ' ' << static_cast < int > (array[0].b) << '\n' <<
        static_cast < int > (array[1].r) << ' ' << static_cast < int > (array[1].g) << ' ' << static_cast < int > (array[1].b) << '\n' <<
        static_cast < int > (array[2].r) << ' ' << static_cast < int > (array[2].g) << ' ' << static_cast < int > (array[2].b) << '\n' <<
        static_cast < int > (array[3].r) << ' ' << static_cast < int > (array[3].g) << ' ' << static_cast < int > (array[3].b) << '\n';
  }
}