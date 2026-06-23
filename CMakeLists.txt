cmake_minimum_required(VERSION 3.28)

project(Raytracer VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)

FetchContent_MakeAvailable(raylib)

add_executable(raytracer src/main.cpp src/RaylibFrontend.cpp)

target_include_directories(raytracer PUBLIC include)
target_link_libraries(raytracer PRIVATE raylib)
