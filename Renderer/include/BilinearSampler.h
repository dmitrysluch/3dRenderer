#pragma once
#include <Eigen/Dense>
#include <algorithm>

#include "Buffer2D.h"
#include "Color.h"
#include "MathHelpers.h"
namespace renderer {
using namespace Eigen;
using namespace std;
class BilinearSampler {
   public:
    static Vector4f Color2Vec(ColorRGBA32 col) { return Vector4f(col.r(), col.g(), col.b(), col.a()) / CHAR_MAX; }
    static Vector4f SamplePixelBilinear(const Buffer2D<ColorRGBA32> &text, Vector2f texcoord);
};
}  // namespace renderer
