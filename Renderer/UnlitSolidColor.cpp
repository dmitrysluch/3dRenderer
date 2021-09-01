#include "include/UnlitSolidColor.h"

using namespace Eigen;

Vector4f renderer::UnlitSolidColor::DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal,
                                              const Vector2f& texcoord, const Vector3f& world_pos,
                                              const Globals* globals) const {
    return color_;
}
