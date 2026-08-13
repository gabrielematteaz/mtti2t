#ifndef MTTI2T_NOISE_FILTER_H_
#define MTTI2T_NOISE_FILTER_H_

#include <cassert>
#include <cstdint>

#include "data_structures\pointer.h"

namespace mtti2t {
  namespace noise_filters {
    ///////////////////////////////////////
    // grayscale noise filters
    ///////////////////////////////////////

    class Median {
      class Histogram {
        int frequencies_[256]{ };
        int total_ = 0;

      public:
        void Add(std::uint8_t value) noexcept {
          ++frequencies_[value];
          ++total_;
        }

        void Remove(std::uint8_t value) noexcept {
          --frequencies_[value];
          --total_;
        }

        void Add(Histogram const& histogram) noexcept {
          for (int index = 0; index < 256; ++index) {
            frequencies_[index] = frequencies_[index] + histogram.frequencies_[index];
          }

          total_ = total_ + histogram.total_;
        }

        void Remove(Histogram const& histogram) noexcept {
          for (int index = 0; index < 256; ++index) {
            frequencies_[index] = frequencies_[index] - histogram.frequencies_[index];
          }

          total_ = total_ - histogram.total_;
        }

        std::uint8_t GetMedian() const noexcept {
          if (total_ == 0) {
            return 0;
          }

          int count = 0;
          int threshold = total_ / 2;

          for (int index = 0; index < 256; ++index) {
            count = count + frequencies_[index];

            if (count > threshold) {
              return static_cast < std::uint8_t > (index);
            }
          }

          return 255;
        }
      };

      int size_;

    public:
      Median(int size) noexcept {
        assert(size > 0);

        size_ = size;
      }

      Pointer < std::uint8_t > operator () (std::uint8_t const* grayscale_data, int width, int height) noexcept;
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
      Pointer < std::uint8_t > operator () (std::uint8_t const* data, int width, int height) noexcept;
    };

    class Erosion {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* data, int width, int height) noexcept;
    };

    // TODO: erosion then dilatation
    class Opening {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* data, int width, int height) noexcept;
    };

    // TODO: dilatation then erosion
    class Closing {
    public:
      Pointer < std::uint8_t > operator () (std::uint8_t const* data, int width, int height) noexcept;
    };
  };
}

#endif