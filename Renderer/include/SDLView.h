#pragma once
#include <SDL2/SDL.h>

#include "Buffer2D.h"
#include "Color.h"
#include "IView.h"

namespace renderer {
class SDLView : public IView {
   public:
    SDLView(SDL_Window *);
    ~SDLView() override;
    void Blit() override;
    [[nodiscard]] Buffer2D<ColorRGBA32>& GetRenderTarget() override { return cpu_render_target_; }
    [[nodiscard]] Buffer2D<float>& GetDepthBuffer() override { return depth_buffer_; }

   private:
    SDL_Window* main_window_;
    SDL_Renderer* main_renderer_;
    SDL_Texture* sdl_render_target_;
    Buffer2D<ColorRGBA32> cpu_render_target_;
    Buffer2D<float> depth_buffer_;
};
}  // namespace renderer