#include "include/Texture2D.h"

using namespace Eigen;

void renderer::Texture2D::SetPixel(int x, int y, const Vector4f& val) {
    auto vec = val * 255.0f;
    int id = GetIndex(x, y);
    arr_[id] = static_cast<unsigned char>(vec.x());
    arr_[id + 1] = static_cast<unsigned char>(vec.y());
    arr_[id + 2] = static_cast<unsigned char>(vec.z());
    arr_[id + 3] = static_cast<unsigned char>(vec.w());
}

Vector4f renderer::Texture2D::operator()(int x, int y) const {
    int id = GetIndex(x, y);
    return Vector4f(static_cast<unsigned char>(arr_[id]), static_cast<unsigned char>(arr_[id + 1]), static_cast<unsigned char>(arr_[id + 2]), static_cast<unsigned char>(arr_[id + 3])) / 255.0f;
}

Vector4f renderer::Texture2D::SamplePixelBilinear(Vector2f a) const {
    float ti = a.x() * (size_.x() - 1);
    int i = floor(ti);
    int ni = min(i + 1, size_.x() - 1);
    ti -= i;
    float tj = a.y() * (size_.y() - 1);
    int j = floor(tj);
    int nj = min(j + 1, size_.y() - 1);
    tj -= j;
    return MathHelpers::Lerp(
        MathHelpers::Lerp((*this)(i, j), (*this)(ni, j), ti),
        MathHelpers::Lerp((*this)(i, nj), (*this)(ni, nj), ti), tj);
}
