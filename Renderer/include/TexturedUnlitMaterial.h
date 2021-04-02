#pragma once
#include "BasicMaterial.h"
#include "BilinearSampler.h"
#include "Buffer2D.h"
#include "Color.h"
namespace renderer {
class TexturedUnlitMaterial : public BasicMaterial {
   public:
    TexturedUnlitMaterial() = delete;
    TexturedUnlitMaterial(const TexturedUnlitMaterial &) = delete;
    TexturedUnlitMaterial& operator = (const TexturedUnlitMaterial &) = delete;
    explicit TexturedUnlitMaterial(const Buffer2D<ColorRGBA32>& texture) : texture_(texture) {}
    explicit TexturedUnlitMaterial(Buffer2D<ColorRGBA32> &&texture) : texture_(texture) {}
    virtual bool RequireNormals() const { return false; }
    virtual bool RequireTexcoords() const { return true; };
    virtual Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal,
                               const Vector2f& texcoord) const {
        return BilinearSampler::SamplePixelBilinear(texture_, texcoord);
    }
private:
    Buffer2D<ColorRGBA32> texture_;
};
}
