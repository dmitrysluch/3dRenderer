#pragma once
#include <algorithm>

#include "BasicMaterial.h"

namespace renderer {
class DrawNormals : public BasicMaterial {
   public:
    [[nodiscard]] bool RequireNormals() const override { return true; }
    [[nodiscard]] bool RequireTexcoords() const override { return false; }
    [[nodiscard]] bool RequireWorldPos() const override { return false; };
    Vector4f DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord,
                       const Vector3f& world_pos, const Globals* globals) const override {
        return Vector4f(normal.x(), normal.y(), normal.z(), 1);
    }
};
}  // namespace renderer
