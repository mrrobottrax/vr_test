project(VrTest VERSION 0.0)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
	add_executable(VrTest WIN32 "pch.h" "app/App.h" "app/App.cpp" "engine/EngineInstance.h" "engine/EngineInstance.cpp" "gl/GlInstance.h" "gl/GlInstance.cpp" "vr/VrInstance.h" "vr/VrInstance.cpp")
endif()

target_link_libraries(VrTest PRIVATE SDL2main)
target_link_libraries(VrTest PRIVATE SDL2-static)
target_link_libraries(VrTest PRIVATE glad)
target_link_libraries(VrTest PRIVATE imgui)

# todo: add all the different openvr versions for each platform
target_link_libraries(VrTest PRIVATE openvr_api64)
target_link_libraries(VrTest PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/openvr/lib/win64/openvr_api.lib" )
add_custom_command(TARGET VrTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/thirdparty/openvr/bin/win64/openvr_api.dll"
        $<TARGET_FILE_DIR:VrTest>)

target_include_directories(VrTest PRIVATE "")

target_compile_options(VrTest PRIVATE /W4 /WX)

target_precompile_headers(VrTest PRIVATE "pch.h")

configure_file(VrTestConfig.h.in VrTestConfig.h)