#ifndef MTTI2T_NOISE_FILTER_H_
#define MTTI2T_NOISE_FILTER_H_

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

    class Opening {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };

    class Closing {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* binarized_data, int width, int height) noexcept;
    };
  };
}

#endif