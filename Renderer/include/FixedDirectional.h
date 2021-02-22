#pragma once
#include "BasicMaterial.h"
#include <algorithm>

namespace renderer {
class FixedLambert : public BasicMaterial {
   public:
    bool RequireNormals() const override {return true; }
    bool RequireTexcoords() const override {return false; }
    Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord) const override {
        auto v3f = Vector3f(219.0/255, 187.0/255, 132.0/255) * (0.1 + std::max(0.0f, Vector3f(0.5, 1, 0.5).normalized().dot(normal.normalized())));
        return Vector4f(v3f.x(), v3f.y(), v3f.z(), 1.0f);
    }
};
}
