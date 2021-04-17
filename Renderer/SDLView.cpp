#include "include/SDLView.h"
#include <fstream>
renderer::SDLView::SDLView(SDL_Window  *window): main_window_(window) {
    main_renderer_ = SDL_CreateRenderer(main_window_, -1, SDL_RENDERER_ACCELERATED);
    int w, h;
    SDL_GetWindowSize(main_window_, &w, &h);
    sdl_render_target_ = SDL_CreateTexture(main_renderer_, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    cpu_render_target_ = Buffer2D<ColorRGBA32>(w, h);
    depth_buffer_ = Buffer2D<float>(w, h);
}
void renderer::SDLView::Blit() {
    void *pixels;
    int pitch;
    SDL_LockTexture(sdl_render_target_, NULL, &pixels, &pitch);
    memcpy(pixels, cpu_render_target_.data(), cpu_render_target_.x() * cpu_render_target_.y() * sizeof(ColorRGBA32));
    SDL_UnlockTexture(sdl_render_target_);
    SDL_RenderCopy(main_renderer_, sdl_render_target_, NULL, NULL);
    SDL_RenderPresent(main_renderer_);
}
renderer::SDLView::~SDLView() {
    SDL_DestroyTexture(sdl_render_target_);
    SDL_DestroyRenderer(main_renderer_);
}
