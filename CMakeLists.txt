cmake_minimum_required(VERSION 3.10)
project(Renderer_cmake)
message (WARNING ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
add_library(Renderer_cmake SHARED
#        Renderer/include/AssimpBindings.h
        Renderer/include/BasicMaterial.h
        Renderer/Camera.cpp
        Renderer/include/Camera.h
        Renderer/DrawNormals.cpp
        Renderer/include/DrawNormals.h
        Renderer/Globals.cpp
        Renderer/include/Globals.h
        Renderer/include/MathHelpers.h
        Renderer/Mesh.cpp
        Renderer/include/Mesh.h
        Renderer/Renderer.cpp
        Renderer/include/Renderer.h
        Renderer/TexturedUnlitMaterial.cpp
        Renderer/include/TexturedUnlitMaterial.h
        Renderer/UnlitSolidColor.cpp
        Renderer/include/UnlitSolidColor.h
        Renderer/include/Buffer2D.h
        Renderer/BilinearSampler.cpp
        Renderer/include/BilinearSampler.h
        Renderer/include/FixedDirectional.h
        Renderer/include/Color.h Renderer/include/SubMesh.h Renderer/include/Object.cpp Renderer/include/Object.h)
include_directories(include)
# compile assimp and copy assimp.lib and assimp.dll (assimp.so and assimp.so.a) to the same folder that executable or just install assimp globally
target_link_libraries(Renderer_cmake assimp)

add_executable(ConsoleRenderer_cmake
        ConsoleRenderer/main.cpp Renderer/include/Kernel.h)

target_include_directories(ConsoleRenderer_cmake PUBLIC Renderer/include)
target_link_libraries(ConsoleRenderer_cmake Renderer_cmake)

file(GLOB FILES_TO_COPY ${PROJECT_SOURCE_DIR}/copy_to_build_dir/*)
file(COPY ${FILES_TO_COPY} DESTINATION ${PROJECT_BINARY_DIR}/)
