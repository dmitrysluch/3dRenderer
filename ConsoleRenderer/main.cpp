#include <AssimpBindings.h>
#include <BasicMaterial.h>
#include <Camera.h>
#include <DrawNormals.h>
#include <FixedDirectional.h>
#include <TexturedUnlitMaterial.h>
#include <Kernel.h>
#include <Object.h>
#include <SDL2/SDL.h>
#include <SDLView.h>
#include <Transform.h>
#include <UnlitSolidColor.h>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace renderer;
using namespace Eigen;

#undef main
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
    SDL_SetMainReady();
    SDL_Window *main_window = SDL_CreateWindow("Renderer from scratch", SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_RESIZABLE);

    KernelInitializer initializer(make_unique<SDLView>(main_window));

    auto main_camera = Camera::New("Main camera");
    main_camera->SetTransform({Vector3f(0, 1, -6), Quaternionf::Identity(), Vector3f(1, 1, 1)});
    int x, y;
    SDL_GetWindowSize(main_window, &x, &y);
    main_camera->SetAspectRatio(static_cast<float>(x) / y);
    main_camera->SetFovAngle(60.f / 180 * AI_MATH_PI_F);
    main_camera->SetNearClipPlane(1);
    main_camera->SetFarClipPlane(100);

    Buffer2D<ColorRGBA32> amogus_texture(512, 512);
    ifstream fin("texture.rgba32");
    fin.read((char *)amogus_texture.data(), 512 * 512 * 4);
    fin.close();

    auto amogus_mesh = AssimpBindings::LoadMeshFromFile("cube.obj", false);
    auto amogus_mat = MaterialVec({make_shared<TexturedUnlitMaterial>(std::move(amogus_texture))});

    vector<Object *> amogus(5);
    for (int i = 0; i < 3; ++i) {
        amogus[i] = Object::New("Amogus " + to_string(i), amogus_mesh, amogus_mat);
        amogus[i]->TransformProxy().SetPosition(Vector3f(i - 1.0, 0, -3));
        amogus[i]->TransformProxy().SetRotation((Quaternionf)AngleAxisf(AI_MATH_PI_F, Vector3f::UnitY()));
    }
    KernelAccessor()->SetActive(true);  // Now Kernel will rerender on each object change
    int curr_amogus = 0;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(main_window);
                SDL_Quit();
                return 0;
            }
            else if (event.type == SDL_KEYDOWN) {  // These is temprorary and should be moved to controllers
                if (event.key.keysym.sym == SDLK_UP) {
                    amogus[curr_amogus]->TransformProxy().SetPosition(amogus[curr_amogus]->GetTransform().position + Vector3f(0, 0, 0.1));
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    amogus[curr_amogus]->TransformProxy().SetPosition(amogus[curr_amogus]->GetTransform().position - Vector3f(0, 0, 0.1));
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    amogus[curr_amogus]->TransformProxy().SetPosition(amogus[curr_amogus]->GetTransform().position - Vector3f(0.1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    amogus[curr_amogus]->TransformProxy().SetPosition(amogus[curr_amogus]->GetTransform().position + Vector3f(0.1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_r) {
                    amogus[curr_amogus]->TransformProxy().SetRotation(amogus[curr_amogus]->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * 0.2, Vector3f::UnitY()));
                } else if (event.key.keysym.sym == SDLK_l) {
                    amogus[curr_amogus]->TransformProxy().SetRotation(amogus[curr_amogus]->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * -0.2, Vector3f::UnitY()));
                } else if (event.key.keysym.sym <= SDLK_3 && event.key.keysym.sym >= SDLK_1) {
                    curr_amogus = event.key.keysym.sym - SDLK_1;
                } else if (event.key.keysym.sym == SDLK_w) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position + Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_s) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position - Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_a) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position - Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_d) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position + Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_j) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position - Vector3f(0, 0.3, 0));
                } else if (event.key.keysym.sym == SDLK_k) {
                    main_camera->TransformProxy().SetPosition(main_camera->GetTransform().position + Vector3f(0, 0.3, 0));
                }
            }
        }
    }
}
