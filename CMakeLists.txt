cmake_minimum_required(VERSION 3.27.2)

project(VrTestParent VERSION 0.0)

include(FetchContent)

FetchContent_Declare(openvr
  GIT_REPOSITORY    https://github.com/ValveSoftware/openvr
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

set(SDL_STATIC TRUE)
set(SDL_SHARED FALSE)
set(SDL_TEST_LIBRARY FALSE)
FetchContent_Declare(sdl
  GIT_REPOSITORY    https://github.com/libsdl-org/SDL
  GIT_TAG			main
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_Declare(imgui
  GIT_REPOSITORY    https://github.com/ocornut/imgui
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(openvr sdl imgui)

add_subdirectory(src)

set(VS_STARTUP_PROJECT VrTest)