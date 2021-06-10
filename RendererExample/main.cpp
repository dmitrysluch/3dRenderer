#include <AssimpBindings.h>
#include <BasicMaterial.h>
#include <BlinnPhong.h>
#include <Camera.h>
#include <DirectionalLight.h>
#include <DrawNormals.h>
#include <Kernel.h>
#include <Keycodes.h>
#include <Library/Observer/Observer.h>
#include <Object.h>
#include <Phong.h>
#include <SDL2/SDL.h>
#include <SDLKeysToRendererKeys.h>
#include <SDLView.h>
#include <TexturedUnlitMaterial.h>
#include <Transform.h>
#include <UnlitSolidColor.h>

#include <fstream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "CameraController.h"
#include "AmogusController.h"

using namespace renderer;
using namespace Eigen;
using namespace std;
using namespace NSLibrary;

int renderer_main(Kernel &kernel) {
    //light
    kernel.Instantiate<DirectionalLight>("Main light", Vector3f(1, -1, 1), Vector3f(1, 1, 1));

    //background
    Buffer2D<ColorRGBA32> background_texture(995, 995);
    ifstream fin("space.rgba32");
    fin.read((char *)background_texture.data(), 995 * 995 * 4);
    fin.close();
    auto skybox_mesh = AssimpBindings::LoadMeshFromFile("skybox.obj");
    auto background_mat = MaterialVec({make_shared<TexturedUnlitMaterial>(std::move(background_texture))});
    auto skybox = kernel.Instantiate<Object>("Skybox", skybox_mesh, background_mat);
    skybox->TransformProxy().SetScale(Vector3f::Ones() * 100);

    auto keyboardObservable = make_unique<CObservableData<KeyCode>>();
    auto camController = CameraController(&kernel, keyboardObservable.get());
    auto amogusController = AmogusController(&kernel, keyboardObservable.get());
    kernel.SetActive(true);  // Now Kernel will rerender on each object change
    while (true) {
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                keyboardObservable->set(SDLKeyCodeToRendrKeyCode(event.key.keysym.sym));
            }
        }
    }
}

#undef main
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw) {
    SDL_SetMainReady();
    SDL_Window *main_window = SDL_CreateWindow("Renderer from scratch", SDL_WINDOWPOS_UNDEFINED,
                                               SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_RESIZABLE);
    Kernel kernel(std::make_unique<SDLView>(main_window));
    renderer_main(kernel);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}
#endif