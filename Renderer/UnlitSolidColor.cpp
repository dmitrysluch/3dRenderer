#include "include/UnlitSolidColor.h"

using namespace Eigen;

Vector4f renderer::UnlitSolidColor::DrawPixel(const Globals* globals, const Vector2i& window_space_vertex,
    const Vector3f& normal, const Vector2f& texcoord) const {
    return color_;
}
