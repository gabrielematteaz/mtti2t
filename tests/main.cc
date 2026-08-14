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

// TODO: optimize by passing optional already allocated memory / decide when it is ok to override

int main(int argc, char * argv[]) {
  if (argc == 1) {
    std::println("No input file provided");
    return 1;
  }

  std::filesystem::path name = argv[1];

  int width, height, depth;
  auto data = stbi_load(argv[1], &width, &height, &depth, 3);

  if (data == NULL || width <= 0 || height <= 0) {
    std::println("An error occurred while loading the provided image");
    return 2;
  }

  mtti2t::RGB::Pixel * RGB_data = reinterpret_cast < mtti2t::RGB::Pixel* > (data);
  auto grayscale_data = mtti2t::grayscale_converters::Recommendation601()(RGB_data, width, height);

  double entropy = mtti2t::image_metrics::GetEntropy(grayscale_data.value(), width, height);
  double noisiness = mtti2t::image_metrics::GetNoisiness(grayscale_data.value(), width, height);
  double bimodality = mtti2t::image_metrics::GetBimodality(grayscale_data.value(), width, height);
  auto [brightness, contrast, cv] = mtti2t::image_metrics::GetBasics(grayscale_data.value(), width, height);
  double edge_density = mtti2t::image_metrics::GetEdgeDensity(grayscale_data.value(), width, height);

  std::println("entropy: {:.03}\nnoisiness: {:.03}\nbimodality: {:.03}\nbrightness: {:.03}\ncontrast: {:.03}\n"
      "cv: {:.03}\nedge density: {:.03}",
      entropy, noisiness, bimodality, brightness, contrast, cv, edge_density);

  if (noisiness > 2.5) {
    std::println("noisy image ... applying median filter");

    grayscale_data = mtti2t::noise_filters::Median(3)(grayscale_data.value(), width, height);
  }
  else if (entropy > 7.0 && bimodality < 0.4) {
    std::println("non-bimodal high entropy image suggests complex background ... applying Gaussian filter");

    grayscale_data = mtti2t::noise_filters::Gaussian(3, 0.0)(grayscale_data.value(), width, height);
  }

  mtti2t::Pointer < std::uint8_t > binary_data;

  if (bimodality > 0.55 && contrast > 35.0) {
    std::println("might be a standard scan ... applying Otsu threshold");

    int otsu_threshold = mtti2t::binarizers::GetOtsuThreshold(grayscale_data.value(), width, height);

    binary_data = mtti2t::binarizers::GlobalThreshold(otsu_threshold)(grayscale_data.value(), width, height);
  }
  else {
    std::print("may be a normal image ... ");

    if (cv < 0.4 || contrast < 20.0) {
      std::println("low variance ... applying Wolf threshold");

      binary_data = mtti2t::binarizers::WolfThreshold(20, 20, 0.1, true)(grayscale_data.value(), width, height);
    }
    else {
      std::println("high variance ... applying Sauvola threshold");

      binary_data = mtti2t::binarizers::SauvolaThreshold(7, 7, 0.2, true)(grayscale_data.value(), width, height);
    }
  }

  if (edge_density > 0.12) {
    std::println("too many edges ... applying dilation");

    binary_data = mtti2t::noise_filters::Dilation()(binary_data.value(), width, height);
  }
  else if (edge_density < 0.02 && brightness < 120) {
    std::println("dark image which low edge density suggest bleeding characters ... applying erosion");

    binary_data = mtti2t::noise_filters::Erosion()(binary_data.value(), width, height);
  }

  std::filesystem::path binary_name = argv[1];
  binary_name.replace_filename("RESULT-BINARY.PNG");
  stbi_write_png(binary_name.string().c_str(), width, height, 1, binary_data.value(), width);

  stbi_image_free(data);
}