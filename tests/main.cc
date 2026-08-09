#include <filesystem>
#include <iostream>
#include <print>

#include "binarizer.h"
#include "RGB.h"
#include "grayscale_converter.h"
#include "noise_filter.h"
#include "image_metrics.h"
#include "text_finder.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char * argv[]) {
  if (argc == 1) {
    std::println("No input file provided");
    return 1;
  }

  int width, height, depth;
  auto data = stbi_load(argv[1], &width, &height, &depth, 3);

  if (data == NULL) {
    std::println("An error occurred while loading the provided image");
    return 2;
  }

  mtti2t::grayscale_converters::Recommendation601 grayscale_converter;
  mtti2t::Pointer < std::uint8_t > grayscale_data = grayscale_converter(reinterpret_cast < mtti2t::RGB* > (data), width, height);
  std::uint8_t * grayscale_data_raw = grayscale_data.value();

  if (grayscale_data_raw == nullptr) {
    std::println("An error occurred while converting the provided image to grayscale");
    stbi_image_free(data);
    return 3;
  }

  std::filesystem::path grayscale_name = argv[1];
  grayscale_name.replace_filename("RESULT-GRAYSCALE.PNG");
  stbi_write_png(grayscale_name.string().c_str(), width, height, 1, grayscale_data_raw, width);

  auto [brightness, contrast] = mtti2t::image_metrics::GetBrightnessAndContrast(grayscale_data_raw, width, height);
  auto noisiness = mtti2t::image_metrics::GetNoisiness(grayscale_data_raw, width, height);

  std::cout << "Brightness: " << brightness << "\nContrast: " << contrast << "\nNoisiness: " << noisiness << '\n';

  mtti2t::binarizers::Binarizer binarizer;

  if (noisiness >= 0 && noisiness < 2) {
    int otsu_threshold = mtti2t::binarizers::GetOtsuThreshold(grayscale_data_raw, width, height);

    std::println("Low noisiness -> Selected Otsu's method\nUsing global threshold: {}", otsu_threshold);
    binarizer = mtti2t::binarizers::GlobalThreshold(otsu_threshold);
  }
  else if (noisiness >= 2 && noisiness < 5) {
    // TODO: let user decide which algorithm to use / auto calculate bets window size and K

    int window_width = 7 * 2 + 1;
    int window_height = 7 * 2 + 1;

    std::println("Medium noisiness -> Selected Sauvola's method\nUsing window size of {}x{} with K = {}\n"
        "Using integral images: {}", window_width, window_height, 0.2, true);
    binarizer = mtti2t::binarizers::SauvolaThreshold(7, 7, 0.2, true);
  }
  else if (noisiness >= 5 && noisiness < 8) {
    // TODO: let user decide which algorithm to use / auto calculate best settings
    // TODO: implement high noise specific algorithms

    std::println("High noise -> ...");
    binarizer = mtti2t::binarizers::SauvolaThreshold(12, 12, 0.5, true); // TEMPORARY
  }
  else {
    std::println("Invalid noisiness provided (must be between 0 and 8)");
    return 4;
  }

  mtti2t::Pointer < std::uint8_t > binary_data = mtti2t::binarizers::Binarize(binarizer, grayscale_data_raw, width, height);
  std::uint8_t * binary_data_raw = binary_data.value();

  if (binary_data_raw == nullptr) {
    std::cout << "binary_data_raw == nullptr";
    stbi_image_free(data);
    return 3;
  }

  std::filesystem::path binary_name = argv[1];
  binary_name.replace_filename("RESULT-BINARY.PNG");
  stbi_write_png(binary_name.string().c_str(), width, height, 1, binary_data_raw, width);

  stbi_image_free(data);
}