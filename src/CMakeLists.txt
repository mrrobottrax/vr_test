project(VrTest VERSION 0.0)

set(SOURCE_FILES "app/App.h" "app/App.cpp")

if (WIN32)
	add_executable(VrTest WIN32 ${SOURCE_FILES})
endif()

target_link_libraries(VrTest PRIVATE SDL2main)
target_link_libraries(VrTest PRIVATE SDL2-static)

target_include_directories(VrTest PRIVATE "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_compile_options(VrTest PRIVATE /W4 /WX)

target_precompile_headers(VrTest PRIVATE "pch.h")

configure_file(VrTestConfig.h.in VrTestConfig.h)