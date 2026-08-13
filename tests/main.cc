#include <filesystem>
#include <iostream>
#include <print>

#include "binarizer.h"
#include "RGB.h"
#include "grayscale_converter.h"
#include "noise_filter.h"
#include "image_metrics.h"
#include "preprocessing.h"
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
  std::print("Loading provided image \"{}\" ... ", name.string()); // TODO: handle Windows and Linux code

  int width, height, depth;
  auto data = stbi_load(argv[1], &width, &height, &depth, 3);

  if (data == NULL || width <= 0 || height <= 0) {
    std::println("An error occurred while loading the provided image");
    return 2;
  }

  std::print("Done\nConverting to grayscale using [...] ... "); // FIX when decision tree is made

  // TODO: decision tree
  mtti2t::grayscale_converters::GrayscaleConverter grayscale_converter;
  mtti2t::Pointer < std::uint8_t > grayscale_data =
      mtti2t::grayscale_converters::ApplyGrayscaleConversion(grayscale_converter,
      reinterpret_cast < mtti2t::RGB::Pixel* > (data), width, height);
  std::uint8_t * grayscale_data_raw = grayscale_data.value();

  if (grayscale_data_raw == nullptr) {
    std::println("An error occurred while converting the provided image to grayscale");
    stbi_image_free(data);
    return 3;
  }

  std::println("Done");

  std::filesystem::path grayscale_name = argv[1];
  grayscale_name.replace_filename("RESULT-GRAYSCALE.PNG");
  stbi_write_png(grayscale_name.string().c_str(), width, height, 1, grayscale_data_raw, width);

  auto basics = mtti2t::image_metrics::GetBasics(grayscale_data_raw, width, height);
  auto entropy = mtti2t::image_metrics::GetEntropy(grayscale_data_raw, width, height);
  auto noisiness = mtti2t::image_metrics::GetNoisiness(grayscale_data_raw, width, height);
  auto bimodality = mtti2t::image_metrics::GetBimodality(grayscale_data_raw, width, height);
  auto edge_density = mtti2t::image_metrics::GetEdgeDensity(grayscale_data_raw, width, height);

  std::cout << "Brightness: " << basics.brightness << "\nContrast: " << basics.contrast << "\nNoisiness: " << noisiness <<
      "\nCoefficient of variation: " << basics.CV << "\nBimodality: " << bimodality <<
      "\nEdge density: " << edge_density << "\nEntropy: " << entropy << '\n';

  mtti2t::binarizers::Binarizer binarizer;

  // NOTE: may be good idea to add a feedback loop if multiple attempts are needed
  // e.g. using canny edge density to decide if good threshold

  if (bimodality > 0.60 && basics.CV < 1.5) {
    int otsu_threshold = mtti2t::binarizers::GetOtsuThreshold(grayscale_data_raw, width, height);

    std::println("Bimodal image -> Selected Otsu's method\nUsing global threshold: {}", otsu_threshold);
    binarizer = mtti2t::binarizers::GlobalThreshold(otsu_threshold);
  }
  else if (noisiness < 15.0) {
    // TODO: let user decide which algorithm to use / auto calculate best window size and K

    int window_width = 7 * 2 + 1;
    int window_height = 7 * 2 + 1;

    std::println("Medium noisiness -> Selected Sauvola's method\nUsing window size of {}x{} with K = {}\n"
        "Using integral images: {}", window_width, window_height, 0.2, true);
    binarizer = mtti2t::binarizers::SauvolaThreshold(7, 7, 0.2, true);
  }
  else if (noisiness >= 15.0) {
    // TODO: let user decide which algorithm to use / auto calculate best settings
    // TODO: implement high noise specific algorithms

    mtti2t::noise_filters::Gaussian noise_filter(5, 1.0);

    std::println("High noise -> Applying a Gaussian filter with a {}x{} kernel and sigma of {}\n"
        "Selected Sauvola's method\nUsing window size of {}x{} and K = {}\nUsing integral images: {}",
        5, 5, 1.0, 12, 12, 0.1, true);
    
    grayscale_data = std::move(noise_filter(grayscale_data_raw, width, height));
    grayscale_data_raw = grayscale_data.value();

    if (grayscale_data_raw == nullptr) {
      std::println("An error occurred while applying a Gaussian filter to the provided image");
      stbi_image_free(data);
      return 4;
    }

    binarizer = mtti2t::binarizers::SauvolaThreshold(12, 12, 0.1, true);
  }
  else {
    std::println("Invalid noisiness provided (must not be less than 0)");
    return 5;
  }

  std::print("Converting to binary ... ");

  mtti2t::Pointer < std::uint8_t > binary_data = mtti2t::binarizers::ApplyBinarization(binarizer,
      grayscale_data_raw, width, height);
  std::uint8_t * binary_data_raw = binary_data.value();

  if (binary_data_raw == nullptr) {
    std::cout << "binary_data_raw == nullptr";
    stbi_image_free(data);
    return 6;
  }

  std::print("Done");

  std::filesystem::path binary_name = argv[1];
  binary_name.replace_filename("RESULT-BINARY.PNG");
  stbi_write_png(binary_name.string().c_str(), width, height, 1, binary_data_raw, width);

  stbi_image_free(data);
}