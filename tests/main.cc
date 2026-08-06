#include <filesystem>
#include <iostream>

#include "binarizer.h"
#include "RGB.h"
#include "grayscale_converter.h"
#include "noise_filter.h"

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
  auto data = stbi_load(argv[1], &width, &height, &depth, 3);

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
  grayscale_name.replace_filename("RESULT-GRAYSCALE.PNG");
  stbi_write_png(grayscale_name.string().c_str(), width, height, 1, grayscale_data_raw, width);

  mtti2t::binarizers::SauvolaThreshold binarizer(12, 12, 0.2, true);
  mtti2t::Pointer < std::uint8_t > binary_data = binarizer(grayscale_data_raw, width, height);
  std::uint8_t * binary_data_raw = binary_data.value();

  if (binary_data_raw == nullptr) {
    std::cout << "binary_data_raw == nullptr";
    stbi_image_free(data);
    return 3;
  }

  std::filesystem::path binary_name = argv[1];
  binary_name.replace_filename("RESULT-BINARY.PNG");
  stbi_write_png(binary_name.string().c_str(), width, height, 1, binary_data_raw, width);

  mtti2t::noise_filters::Erosion noise_filter1;
  mtti2t::Pointer < std::uint8_t > filtered_data1 = noise_filter1(binary_data_raw, width, height);
  std::uint8_t * filtered_data_raw1 = filtered_data1.value();

  if (filtered_data_raw1 == nullptr) {
    std::cout << "filtered_data_raw == nullptr";
    stbi_image_free(data);
    return 4;
  }

  std::filesystem::path filtered_name1 = argv[1];
  filtered_name1.replace_filename("RESULT-FILTERED1.PNG");
  stbi_write_png(filtered_name1.string().c_str(), width, height, 1, filtered_data_raw1, width);

  mtti2t::noise_filters::Dilatation noise_filter2;
  mtti2t::Pointer < std::uint8_t > filtered_data2 = noise_filter2(binary_data_raw, width, height);
  std::uint8_t * filtered_data_raw2 = filtered_data2.value();

  if (filtered_data_raw2 == nullptr) {
    std::cout << "filtered_data_raw == nullptr";
    stbi_image_free(data);
    return 4;
  }

  std::filesystem::path filtered_name2 = argv[1];
  filtered_name2.replace_filename("RESULT-FILTERED2.PNG");
  stbi_write_png(filtered_name2.string().c_str(), width, height, 1, filtered_data_raw2, width);

  stbi_image_free(data);
}