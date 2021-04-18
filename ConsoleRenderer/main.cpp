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
    mainCamera->SetTransform({Vector3f(0, 0.1, -12), Quaternionf::Identity(), Vector3f(1, 1, 1)});
    mainCamera->SetAspectRatio(4.0 / 3);
    mainCamera->SetFovAngle(60.f / 180 * AI_MATH_PI_F);
    mainCamera->SetNearClipPlane(0.1);
    mainCamera->SetFarClipPlane(100);

    auto dickMesh = AssimpBindings::LoadMeshFromFile("mesh.fbx", false);
    auto color = Eigen::Vector4f(1.f, 1.f, 1.f, 1.f);
    auto normalsMat = make_shared<const DrawNormals>();
    auto dick = Object::New("Dildo", dickMesh, MaterialVec({normalsMat}));
    dick->TransformProxy().SetPosition(Vector3f(0, 0, 4));
    dick->TransformProxy().SetRotation(static_cast<Quaternionf>(AngleAxisf(1.1f * AI_MATH_PI, Vector3f::UnitY())));
   // dick->TransformProxy().SetScale(Vector3f::Ones() * 0.3);
    KernelAccessor()->SetActive(true);  // Now Kernel will rerender on each object change

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
                    dick->TransformProxy().SetPosition(dick->GetTransform().position + Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    dick->TransformProxy().SetPosition(dick->GetTransform().position - Vector3f(0, 0, 1));
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    dick->TransformProxy().SetPosition(dick->GetTransform().position - Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    dick->TransformProxy().SetPosition(dick->GetTransform().position + Vector3f(1, 0, 0));
                } else if (event.key.keysym.sym == SDLK_r) {
                    dick->TransformProxy().SetRotation(dick->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * 0.2, Vector3f::UnitY()));
                } else if (event.key.keysym.sym == SDLK_l) {
                    dick->TransformProxy().SetRotation(dick->GetTransform().rotation *
                                                       AngleAxisf(AI_MATH_PI_F * -0.2, Vector3f::UnitY()));
                    }
            }
        }
    }
}
