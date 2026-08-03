#include <filesystem>
#include <iostream>

#include "binarizer.h"
#include "RGB.h"
#include "grayscale_converter.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char * argv[]) {
  if (argc == 1) {
    std::cout << "argc == 1";
    return 1;
  }

  int width, height, depth;
  auto data = stbi_load(argv[1], &width, &height, &depth, 0);

  if (data == NULL) {
    std::cout << "data == NULL";
    return 2;
  }

  mtti2t::grayscale_converters::Recommendation601 grayscale_converter;
  mtti2t::Pointer < std::uint8_t > grayscale_data = grayscale_converter(reinterpret_cast < mtti2t::RGB* > (data), width, height);
  std::uint8_t * grayscale_data_raw = grayscale_data.value();

  if (grayscale_data_raw == nullptr) {
    std::cout << "grayscale_data_raw == nullptr";
    stbi_image_free(data);
    return 3;
  }

  std::filesystem::path grayscale_name = argv[1];
  grayscale_name.replace_filename("RESULT-GRAYSCALE.BMP");
  stbi_write_bmp(grayscale_name.string().c_str(), width, height, 1, grayscale_data_raw);

  mtti2t::binarizers::SauvolaThreshold binarizer(15, 15, 0.4);
  mtti2t::Pointer < std::uint8_t > binary_data = binarizer(grayscale_data_raw, width, height);
  std::uint8_t * binary_data_raw = binary_data.value();

  if (binary_data_raw == nullptr) {
    std::cout << "binary_data_raw == nullptr";
    stbi_image_free(data);
    return 3;
  }

  std::filesystem::path binary_name = argv[1];
  binary_name.replace_filename("RESULT-BINARY.BMP");
  stbi_write_bmp(binary_name.string().c_str(), width, height, 1, binary_data_raw);

  stbi_image_free(data);
}