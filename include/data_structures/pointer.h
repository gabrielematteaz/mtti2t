#ifndef MTTI2T_INCLUDE_DATA_STRUCTURES_POINTER_H_
#define MTTI2T_INCLUDE_DATA_STRUCTURES_POINTER_H_

#include <cassert>
#include <new>

namespace mtti2t {
  template < typename ValueType >
  class Pointer {
      ValueType * value_;

    public:
      Pointer(const Pointer &) = delete;
      Pointer &operator = (const Pointer &) = delete;

      Pointer() noexcept {
        value_ = nullptr;
      }

      explicit Pointer(int count) noexcept {
        assert(count >= 0);

        value_ = new(std::nothrow) ValueType[count];
      }

      Pointer(Pointer &&other) noexcept {
        value_ = other.value_;
        other.value_ = nullptr;
      }

      ~Pointer() {
        Destroy();
      }

      Pointer &operator = (Pointer &&other) noexcept {
        if (this != &other) {
          Destroy();
          value_ = other.value_;
          other.value_ = nullptr;
        }

        return *this;
      }

      ValueType * value() const noexcept {
        return value_;
      }

    private:
      void Destroy() noexcept {
        delete[] value_;
      }
  };
}

#endif