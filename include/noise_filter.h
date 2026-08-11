#ifndef MTTI2T_NOISE_FILTER_H_
#define MTTI2T_NOISE_FILTER_H_

#include <cassert>
#include <cmath>
#include <cstdint>

#include "data_structures\pointer.h"

namespace mtti2t {
  namespace noise_filters {
    ///////////////////////////////////////
    // grayscale noise filters
    ///////////////////////////////////////

    // TODO: implement median and gaussian de-noising
    // TODO: add a radius -> perreault or huang algorithm

    class Median {

    };

    class Gaussian {
      int size_;
      double sigma_;

    public:
      Gaussian(int size, double sigma) noexcept {
        assert(size > 0);

        size_ = size;
        sigma_ = sigma;
      }

      Pointer < std::uint8_t > operator () (std::uint8_t const* grayscale_data, int width, int height) noexcept;

      static Pointer < double > GetKernel(int size, double sigma) noexcept;
    };

    ///////////////////////////////////////
    // binary noise filters
    ///////////////////////////////////////

    // TODO: add a radius -> van herk / gil-weman optimization
    // TODO: optimize opening and closing

    class Dilatation {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };

    class Erosion {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };

    // TODO: erosion then dilatation
    class Opening {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };

    // TODO: dilatation then erosion
    class Closing {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };
  };
}

#endif