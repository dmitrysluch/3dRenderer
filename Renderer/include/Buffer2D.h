#pragma once
#include <exception>
#include <vector>

namespace renderer {
using namespace std;
/// class for 2d array which is stored as flat in memory.
/// The x axis if first and is directed rightwards while y is directed upwards.
/// The first coordinate of access operators is x and the the lowest row has y coordinate = 0.
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
        for (int i = y_ - 1; i >= 0; --i) {
            assert(nested[i].size() == x_ && "sizes of slices don't match");
            copy(nested[i].begin(), nested[i].end(), flat_.begin() + i * x_);
        }
    }
    T &operator()(size_t x, size_t y) { return flat_[(y_ - 1 - y) * x_ + x]; }
    const T &operator()(size_t x, size_t y) const { return flat_[(y_ - 1 - y) * x_ + x]; }
    const T *data() const noexcept { return flat_.data(); }
    T *data() noexcept { return flat_.data(); }
    [[nodiscard]] inline size_t x() const { return x_; }
    [[nodiscard]] inline size_t y() const { return y_; }

   private:
    size_t x_, y_;
    vector<T> flat_;
};
}  // namespace renderer
