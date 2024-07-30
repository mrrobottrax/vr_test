project(imgui)

add_library(imgui "imgui.cpp" "imgui_demo.cpp" "imgui_draw.cpp" "imgui_tables.cpp" "imgui_widgets.cpp" "backends/imgui_impl_opengl3.cpp" "backends/imgui_impl_sdl2.cpp")
target_include_directories(imgui PUBLIC "" "backends")

target_link_libraries(imgui PRIVATE SDL2-static)