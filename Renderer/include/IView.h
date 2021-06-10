#pragma once
#include "Buffer2D.h"
#include "Color.h"

namespace renderer {
class IView {
   public:
    virtual ~IView() = default;
    virtual Buffer2D<ColorRGBA32>& GetRenderTarget() = 0;
    virtual Buffer2D<float>& GetDepthBuffer() = 0;
    virtual void Blit() = 0;
};
}
