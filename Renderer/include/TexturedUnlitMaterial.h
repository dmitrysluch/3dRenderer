#pragma once
#include "BasicMaterial.h"
#include "Texture2D.h"
namespace renderer {
class TexturedUnlitMaterial : public BasicMaterial {
   public:
    explicit TexturedUnlitMaterial(const Texture2D& texture) : texture_(texture) {}
    explicit TexturedUnlitMaterial(Texture2D &&texture) : texture_(texture) {}
    virtual bool RequireNormals() const { return false; }
    virtual bool RequireTexcoords() const { return true; };
    virtual Vector4f DrawPixel(const Globals* globals, const Vector2i& window_space_vertex, const Vector3f& normal,
                               const Vector2f& texcoord) const {
        return texture_.SamplePixelBilinear(texcoord);
    }
private:
    Texture2D texture_;
};
}
