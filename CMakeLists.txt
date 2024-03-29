cmake_minimum_required(VERSION 3.10)
project(Renderer_cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
if (MSVC)
    add_compile_options(/openmp /DEIGEN_DONT_PARALLELIZE) # Causes races. IDGAD
endif()
add_library(Renderer_cmake SHARED
        Renderer/include/AssimpBindings.h
        Renderer/include/BasicMaterial.h
        Renderer/Camera.cpp
        Renderer/include/Camera.h
        Renderer/include/DrawNormals.h
        Renderer/MathHelpers.cpp
        Renderer/include/MathHelpers.h
        Renderer/Mesh.cpp
        Renderer/include/Mesh.h
        Renderer/Renderer.cpp
        Renderer/include/Renderer.h
        Renderer/include/TexturedUnlitMaterial.h
        Renderer/UnlitSolidColor.cpp
        Renderer/include/UnlitSolidColor.h
        Renderer/include/Buffer2D.h
        Renderer/BilinearSampler.cpp
        Renderer/include/BilinearSampler.h
        Renderer/include/Color.h
        Renderer/include/SubMesh.h
        Renderer/Object.cpp
        Renderer/include/Object.h
        Renderer/include/Kernel.h
        Renderer/Kernel.cpp
        Renderer/IView.cpp
        Renderer/include/IView.h Renderer/include/Transform.h Renderer/Transform.cpp Renderer/SDLView.cpp Renderer/include/SDLView.h Renderer/include/Keycodes.h Renderer/include/SDLKeysToRendererKeys.h Renderer/BaseController.cpp Renderer/include/BaseController.h Renderer/DirectionalLight.cpp Renderer/include/DirectionalLight.h Renderer/Phong.cpp Renderer/include/Phong.h Renderer/include/Globals.h Renderer/BlinnPhong.cpp Renderer/include/BlinnPhong.h)
include_directories(include)
# compile assimp and copy assimp.lib and assimp.dll (assimp.so and assimp.so.a) to the same folder that executable or just install assimp globally
target_link_libraries(Renderer_cmake assimp SDL2)

add_executable(RendererExample_cmake WIN32
        RendererExample/main.cpp RendererExample/CameraController.cpp RendererExample/CameraController.h RendererExample/CameraController.cpp RendererExample/AmogusController.cpp RendererExample/AmogusController.h)

target_include_directories(RendererExample_cmake PUBLIC Renderer/include)
target_link_libraries(RendererExample_cmake Renderer_cmake)

add_executable(RendererTests_cmake RendererTests/CameraTest.cpp)
target_include_directories(RendererTests_cmake PUBLIC Renderer/include gtest/include)
target_link_libraries(RendererTests_cmake Renderer_cmake gtestd gtest_maind)


file(GLOB FILES_TO_COPY ${PROJECT_SOURCE_DIR}/copy_to_build_dir/*)
file(COPY ${FILES_TO_COPY} DESTINATION ${PROJECT_BINARY_DIR}/)
