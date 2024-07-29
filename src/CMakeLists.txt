project(VrTest VERSION 0.0)

if (WIN32)
	add_executable(VrTest WIN32 "main.cpp" "main.h" )
endif()

target_link_libraries(VrTest PRIVATE SDL2main)
target_link_libraries(VrTest PRIVATE SDL2-static)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_compile_options(VrTest PRIVATE /W4 /WX)

configure_file(VrTestConfig.h.in VrTestConfig.h)