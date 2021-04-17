#pragma once
#include <exception>
#include <vector>

namespace renderer {
using namespace std;

template <typename T>
class Buffer2D {
   public:
    Buffer2D(): x_(0), y_(0){}
    Buffer2D(size_t x, size_t y) : x_(x), y_(y), flat_(x * y) {}
    Buffer2D(size_t x, const vector<T> &flat) : x_(x), flat_(flat) {
        y_ = flat_.size() / x_;
        assert (y_ * x_ == flat.size() && "x must be divisor of flat array size");
    }
    Buffer2D(size_t x, vector<T> &&flat) : x_(x), flat_(flat) {
        y_ = flat_.size() / x_;
        assert (y_ * x_ == flat.size() && "x must be divisor of flat array size");
    }
    Buffer2D(const vector<vector<T>> &nested) : x_(nested.size()) {
        if (!x_) {
            y_ = 0;
            return;
        }
        y_ = nested[0].size();
        flat_.resize(x_ * y_);
        for (int i = 0; i < x_; ++i) {
            assert(nested[i].size() == y_ && "sizes of slices don't match");
            copy(nested[i].begin(), nested[i].end(), flat_.begin() + i * y_);
        }
    }
    T &operator()(size_t i, size_t j) { return flat_[i * y_ + j]; }
    const T &operator()(size_t i, size_t j) const { return flat_[i * y_ + j]; }
    const T *data() const noexcept { return flat_.data(); }
    T *data() noexcept { return flat_.data(); }
    [[nodiscard]] inline size_t x() const { return x_; }
    [[nodiscard]] inline size_t y() const { return y_; }

   private:
    size_t x_, y_;
    vector<T> flat_;
};
}  // namespace renderer
