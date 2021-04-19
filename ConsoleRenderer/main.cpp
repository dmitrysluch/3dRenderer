#include <AssimpBindings.h>
#include <BasicMaterial.h>
#include <Camera.h>
#include <DrawNormals.h>
#include <FixedDirectional.h>
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
    SDL_Window *main_window_ = SDL_CreateWindow("Renderer from scratch", SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);

    KernelInitializer initializer(make_unique<SDLView>(main_window_));

    auto mainCamera = Camera::New("Main camera");
    mainCamera->SetTransform({Vector3f(0, 0, -6), Quaternionf::Identity(), Vector3f(1, 1, 1)});
    mainCamera->SetAspectRatio(4.0 / 3);
    mainCamera->SetFovAngle(60.f / 180 * AI_MATH_PI_F);
    mainCamera->SetNearClipPlane(0.1);
    mainCamera->SetFarClipPlane(100);

    auto amogusMesh = AssimpBindings::LoadMeshFromFile("amogus.obj", false);
    auto amogusMat = MaterialVec({make_shared<FixedLambert>()});

    vector<Object *> amogus(5);
    for (int i = 0; i < 3; ++i) {
        amogus[i] = Object::New("Amogus " + to_string(i), amogusMesh, amogusMat);
        amogus[i]->TransformProxy().SetPosition(Vector3f((i - 1) * 3, 0, 0));
    }
    KernelAccessor()->SetActive(true);  // Now Kernel will rerender on each object change
    int currAmogus = 0;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(main_window_);
                SDL_Quit();
                return 0;
            }
            else if (event.type == SDL_KEYDOWN) {  // These is temprorary and should be moved to controllers
                if (event.key.keysym.sym == SDLK_UP) {
                    amogus[currAmogus]->TransformProxy().SetPosition(amogus[currAmogus]->GetTransform().position + Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    amogus[currAmogus]->TransformProxy().SetPosition(amogus[currAmogus]->GetTransform().position - Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    amogus[currAmogus]->TransformProxy().SetPosition(amogus[currAmogus]->GetTransform().position - Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    amogus[currAmogus]->TransformProxy().SetPosition(amogus[currAmogus]->GetTransform().position + Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_r) {
                    amogus[currAmogus]->TransformProxy().SetRotation(amogus[currAmogus]->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * 0.2, Vector3f::UnitY()));
                } else if (event.key.keysym.sym == SDLK_l) {
                    amogus[currAmogus]->TransformProxy().SetRotation(amogus[currAmogus]->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * -0.2, Vector3f::UnitY()));
                } else if (event.key.keysym.sym <= SDLK_3 && event.key.keysym.sym >= SDLK_1) {
                    currAmogus = event.key.keysym.sym - SDLK_1;
                } else if (event.key.keysym.sym == SDLK_w) {
                    mainCamera->TransformProxy().SetPosition(mainCamera->GetTransform().position + Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_s) {
                    mainCamera->TransformProxy().SetPosition(mainCamera->GetTransform().position - Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_a) {
                    mainCamera->TransformProxy().SetPosition(mainCamera->GetTransform().position - Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_d) {
                    mainCamera->TransformProxy().SetPosition(mainCamera->GetTransform().position + Vector3f(1, 0, 0));
                }
            }
        }
    }
}
