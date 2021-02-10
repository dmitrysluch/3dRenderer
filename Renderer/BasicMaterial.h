#pragma once
#include <Eigen/Dense>
#include "Globals.h"

namespace renderer {
using namespace  Eigen;

class BasicMaterial abstract {
   public:
    virtual ~BasicMaterial() {}
    virtual bool RequireNormals() const = 0;
    virtual bool RequireTexcoords() const = 0;
    virtual Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord) const = 0;
};
}
