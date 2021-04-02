#pragma once
#include "BasicMaterial.h"

namespace renderer {
class UnlitSolidColor : public BasicMaterial {
   public:
    UnlitSolidColor() = delete;
    UnlitSolidColor(const UnlitSolidColor &) = delete;
    UnlitSolidColor &operator=(const UnlitSolidColor &) = delete;
    explicit UnlitSolidColor(const Vector4f& color)
        : color_(color) {
    }

    [[nodiscard]] Vector4f GetColor() const { return color_; }

    [[nodiscard]] bool RequireNormals() const override { return false; }
    [[nodiscard]] bool RequireTexcoords() const override { return false; }
    Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal,
                       const Vector2f& texcoord) const override;

   private:
    Vector4f color_;
};
}  // namespace renderer
