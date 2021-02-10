#pragma once
#include "BasicMaterial.h"

namespace renderer {
class DrawNormals : public BasicMaterial {
public:
    bool RequireNormals() const override {return true; }
    bool RequireTexcoords() const override {return false; }
    Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord) const override {
        return Vector4f(normal.x(), normal.y(), normal.z(), 1);
    }
};
}
