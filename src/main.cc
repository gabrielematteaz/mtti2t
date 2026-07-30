#include <iostream>

#include "binarizer.h"
#include "BMP.h"
#include "grayscale_converter.h"
#include "text_finder.h"

int main() {
  mtti2t::BMP::Pixel pixels[] = {
    100, 200, 150,0, 240, 140, 150,0, 180,  10, 130,0, 100, 200, 150,0,
    255, 255, 255,0,  60, 166, 150,0, 176,  80, 130,0, 100, 200, 150,0,
      0,   0,   0,0,  50, 200, 255,0, 100,  90, 150,0, 100, 200, 150,0,
     10,  20, 200,0, 100,   0, 150,0,  90,  90, 150,0, 100, 200, 150,0,
  };

  auto result = mtti2t::GrayscaleConverter::Rec601(pixels, 4, 4);

  if (result == nullptr) {
    return 1;
  }

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      int index = y * 4 + x;

      std::cout << (int)result[index] << ' ';
    }

    std::cout << '\n';
  }

  mtti2t::MeanBinarizer binarizer(2, 2);

  if (binarizer(result, 4, 4) == false) {
    return 2;
  }

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      int index = y * 4 + x;

      std::cout << (int)result[index] << ' ';
    }

    std::cout << '\n';
  }

  auto rectangles = mtti2t::TextFinder::Histogram(result, 4, 4);

  for (auto const& rectangle : rectangles) {
    std::cout << rectangle.x << ' ' << rectangle.y << ' ' << rectangle.x2 << ' ' << rectangle.y2 << '\n';
  }

  delete[] result;
}