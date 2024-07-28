project(VrTest VERSION 0.0)

# add_executable(VrTest "engine/EngineInstance.cpp" "engine/EngineInstance.h" "files/FileManager.cpp" "files/FileManager.h" "gl/GlDebug.h" "gl/GlInstance.cpp" "gl/GlInstance.h" "gl/RenderTarget.cpp" "gl/RenderTarget.h" "gl/ShaderProgram.cpp" "gl/ShaderProgram.h" "vr/VrInstance.cpp" "vr/VrInstance.h" "windows/WinApp.cpp" "windows/WinApp.h" "windows/Window.cpp" "windows/Window.h" "App.h")

add_executable(VrTest WIN32 "main.cpp" "main.h" )

add_dependencies(VrTest openvr_api64)
add_dependencies(VrTest SDL3::SDL3)
add_dependencies(VrTest imgui)

target_link_libraries(VrTest PRIVATE SDL3::SDL3 openvr_api64 imgui)
#target_include_directories(VrTest PUBLIC "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(VrTestConfig.h.in VrTestConfig.h)