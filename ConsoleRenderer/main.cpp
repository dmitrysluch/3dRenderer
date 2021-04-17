#include <AssimpBindings.h>
#include <Camera.h>
#include <DrawNormals.h>
#include <UnlitSolidColor.h>
#include <Kernel.h>
#include <Object.h>
#include <SDL2/SDL.h>
#include <SDLView.h>
#include <windows.h>
#include <sstream>
#include <BasicMaterial.h>
#include <Transform.h>

using namespace renderer;
using namespace Eigen;

#undef main
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
    SDL_SetMainReady();
    SDL_Window *main_window_ = SDL_CreateWindow("Renderer from scratch", SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
    auto view = make_unique<SDLView>(main_window_);
    Renderer renderer;
    renderer.SetRenderTarget(&view->GetRenderTarget());
    renderer.SetDepthBuffer(&view->GetDepthBuffer());
    renderer.SetViewport(Vector2i(view->GetRenderTarget().x(), view->GetRenderTarget().y()));

    auto dickMesh = AssimpBindings::LoadMeshFromFile("Dildo.OBJ", false);
    auto color = Eigen::Vector4f(1.f, 1.f, 1.f, 1.f);
    auto mat = new UnlitSolidColor(color);

//    renderer.SetVertices(&dickMesh->GetSubmesh(0).vertices);
//    renderer.SetIndices(&dickMesh->GetSubmesh(0).indices);
//    renderer.SetMaterial(mat);
//
//    renderer.SetObjectTransform(Projective3f(Translation3f(0, 0, 4) * AngleAxisf(1.1f * AI_MATH_PI, Vector3f::UnitY())).matrix());
//    renderer.SetEyeAndProjectionMatrices(static_cast<Projective3f>(Translation3f(0, 2, -12)).inverse().matrix(), MathHelpers::GetProjectionMatrix(30, 4.0/4, 100, 0.01));
//
//    renderer.Clear(ColorRGBA32({100, 100, (char)200, (char)225}));
//    renderer.DrawIndexed();
    view->Blit();
//    KernelInitializer initializer(make_unique<SDLView>(main_window_));
//
//    auto mainCamera = Camera::New("Main camera");
//    mainCamera->SetTransform({Vector3f(0, 2, -12), Quaternionf::Identity(), Vector3f::Ones()});
//    mainCamera->SetAspectRatio(4.0/3);
//    mainCamera->SetFovAngle(60);
//    mainCamera->SetNearClipPlane(0.1);
//    mainCamera->SetFarClipPlane(100);
//
//    auto dickMesh = AssimpBindings::LoadMeshFromFile("Dildo.OBJ", false);
//    auto color = Eigen::Vector4f(1.f, 1.f, 1.f, 1.f);
//    auto  mat = new UnlitSolidColor(color);
//    auto normalsMat = make_shared<const DrawNormals>();
//    auto dick = Object::New("Dildo", dickMesh, MaterialVec({normalsMat}));
//    dick->TransformProxy().SetPosition(Vector3f(0, 0, 4));
//    dick->TransformProxy().SetRotation(static_cast<Quaternionf>(AngleAxisf(1.1f * AI_MATH_PI, Vector3f::UnitY())));
////    for (int i = 0; i < 1; ++i) {
////        ostringstream name;
////        name << "Dick " << i;
////        auto obj = Object::New(name.str(), dickMesh, MaterialVec({normalsMat}));
////        obj->TransformProxy().SetPosition(Vector3f(i - 2.5f, 0.f, 0.f));
////    }
//
//    KernelAccessor()->SetActive(true); // Now Kernel will rerender on each object change

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(main_window_);
                SDL_Quit();
                return 0;
            }

//            else if (event.type == SDL_KEYDOWN) { // These is temprorary and should be moved to controllers
//                if (event.key.keysym.sym == SDLK_UP) {
//                    dick->TransformProxy().SetPosition(dick->GetTransform().position +  Vector3f(0, 0, 1));
//                } else if (event.key.keysym.sym == SDLK_DOWN) {
//                    dick->TransformProxy().SetPosition(dick->GetTransform().position -  Vector3f(0, 0, 1));
//                } else if (event.key.keysym.sym == SDLK_LEFT) {
//                    dick->TransformProxy().SetPosition(dick->GetTransform().position -  Vector3f(1, 0, 0));
//                } else if (event.key.keysym.sym == SDLK_RIGHT) {
//                    dick->TransformProxy().SetPosition(dick->GetTransform().position +  Vector3f(1, 0, 0));
//                }
//            }
        }
    }
}
