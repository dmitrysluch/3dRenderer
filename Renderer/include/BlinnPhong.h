#pragma once
#include <iostream>
#include <variant>

#include "BasicMaterial.h"
#include "BilinearSampler.h"
#include "Buffer2D.h"
#include "Color.h"
#include "MathHelpers.h"

namespace renderer {
class BlinnPhong : public BasicMaterial {
   public:
    BlinnPhong() = delete;
    BlinnPhong(const BlinnPhong&) = delete;
    BlinnPhong& operator=(const BlinnPhong&) = delete;
    BlinnPhong(const Buffer2D<ColorRGBA32>& albedo, float specular_power, float specular_multiplier)
        : albedo_(albedo), specular_power_(specular_power), specular_multiplier_(specular_multiplier) {}
    BlinnPhong(Buffer2D<ColorRGBA32>&& albedo, float specular_power, float specular_multiplier)
        : albedo_(std::move(albedo)), specular_power_(specular_power), specular_multiplier_(specular_multiplier) {}
    [[nodiscard]] bool RequireNormals() const override { return true; }
    [[nodiscard]] bool RequireTexcoords() const override { return true; };
    [[nodiscard]] bool RequireWorldPos() const override { return true; };
    [[nodiscard]] Vector4f DrawPixel(const Vector2i& window_space_vertex, const Vector3f& normal,
                                     const Vector2f& texcoord, const Vector3f& world_pos,
                                     const Globals* globals) const override;

   private:
    Buffer2D<ColorRGBA32> albedo_;
    float specular_power_, specular_multiplier_;
};
}  // namespace renderer
