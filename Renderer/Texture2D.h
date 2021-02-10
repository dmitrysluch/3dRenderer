#pragma once
#include <algorithm>
#include "MathHelpers.h"

namespace renderer {
using namespace Eigen;
using namespace std;


class Texture2D {
   public:
    Texture2D(int x, int y) : size_(x, y){ arr_ = new char[x * y * kPixelSize]; }
    Texture2D(const Texture2D &other) : size_(other.size_) {
        arr_ = new char[size_.x() * size_.y() * kPixelSize];
        memcpy(arr_, other.arr_, size_.x() * size_.y() * kPixelSize);
    }
    Texture2D(Texture2D &&other) noexcept : arr_(nullptr), size_(other.size_) {
        swap(arr_, other.arr_);
    }
    ~Texture2D() { delete[] arr_; }
    Texture2D &operator= (const Texture2D &other) {
        if (&other == this) {
            return *this;
        }
        delete[] arr_;
        size_ = other.size_;
        memcpy(arr_, other.arr_, size_.x() * size_.y() * kPixelSize);
        return *this;
    }
    Texture2D &operator=(Texture2D &&other) noexcept {
        size_ = other.size_;
        swap(arr_, other.arr_);
        return *this;
    }
    char *RawData() const { return arr_; }
    Vector2i GetSize() const { return size_; }
    // integer not normalized coordinates
    inline void SetPixel(int x, int y, const Vector4f& val);
    inline Vector4f operator()(int x, int y) const;
    // normalized float coordinates
    Vector4f SamplePixelBilinear(Vector2f a) const;
    friend void swap(Texture2D &lhv, Texture2D &rhv) {
        swap(lhv.size_, rhv.size_);
        swap(lhv.arr_, rhv.arr_);
    }
   private:
    static constexpr int kPixelSize = 4;
    char *arr_;
    Vector2i size_;

    inline int GetIndex(int x, int y) const { return ((size_.y() - y - 1) * size_.x() + x) * kPixelSize; }
};
}  // namespace renderer
