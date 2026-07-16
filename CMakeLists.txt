cmake_minimum_required(VERSION 3.28)

project(Raytracer VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
)

FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(glm)

add_executable(raytracer src/main.cpp 
src/RaylibFrontend.cpp 
src/Raytracer.cpp 
src/Ray.cpp 
src/Sphere.cpp 
src/IHit.cpp
src/Interval.cpp
src/Camera.cpp
src/AssetManager.cpp
src/LambertianMaterial.cpp
src/MetalMaterial.cpp
src/DielectricMaterial.cpp
src/Image.cpp
src/BVHNode.cpp
src/World.cpp)

target_include_directories(raytracer PRIVATE include external)
target_link_libraries(raytracer PRIVATE raylib glm)

file(COPY
    ${CMAKE_SOURCE_DIR}/resources
    DESTINATION ${CMAKE_BINARY_DIR}
)
