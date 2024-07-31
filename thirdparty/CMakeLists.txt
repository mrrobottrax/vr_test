set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL2_DISABLE_UNINSTALL ON)
set(SDL_TEST OFF)
set(SDL_LIBC ON)
add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_subdirectory(glad EXCLUDE_FROM_ALL)

add_subdirectory(imgui EXCLUDE_FROM_ALL)

set(BUILD_UNIVERSAL OFF)
add_subdirectory(openvr EXCLUDE_FROM_ALL)