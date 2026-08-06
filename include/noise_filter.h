#ifndef MTTI2T_NOISE_FILTER_H_
#define MTTI2T_NOISE_FILTER_H_

#include "data_structures\pointer.h"

namespace mtti2t {
  namespace noise_filters {
    // might be good idea to add a radius -> optimize with integral image
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