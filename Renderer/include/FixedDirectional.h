#pragma once
#include <BilinearSampler.h>
#include <Color.h>

#include <algorithm>
#include <iostream>

#include "BasicMaterial.h"

namespace renderer {
class FixedLambert : public BasicMaterial {
   public:
    bool RequireNormals() const override {return true; }
    bool RequireTexcoords() const override {return false; }
    Vector4f DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord) const override {
        auto v3f = Vector3f::Ones() * (0.1 + std::max<float>(0.0f, Vector3f(0.5, 1, 0.5).normalized().dot(normal.normalized())));
        return Vector4f(v3f.x(), v3f.y(), v3f.z(), 1.0f);
    }
};
class FixedLambertTextured : public BasicMaterial {

   public:
    FixedLambertTextured(Buffer2D<ColorRGBA32> &&diffuse): diffuse_(diffuse){ }
    bool RequireNormals() const override {return false; }
    bool RequireTexcoords() const override {return true; }
    Vector4f DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal, const Vector2f& texcoord) const override {
//        auto v3f = BilinearSampler::SamplePixelBilinear(diffuse_, texcoord) * (0.1 + std::max<float>(0.0f, Vector3f(0.5, 1, 0.5).normalized().dot(normal.normalized())));
        return Vector4f(texcoord.x(), texcoord.y(), 0, 1.0f);
    }
   private:
    Buffer2D<ColorRGBA32> diffuse_;
};
}
