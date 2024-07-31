project(VrTest VERSION 0.0)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES "pch.h" "app/App.h" "app/App.cpp" "engine/EngineInstance.h" "engine/EngineInstance.cpp" "gl/GlInstance.h" "gl/GlInstance.cpp" "gl/RenderTarget.h" "gl/RenderTarget.cpp" "gl/ShaderProgram.h" "gl/ShaderProgram.cpp" "vr/VrInstance.h" "vr/VrInstance.cpp")

# todo: add different platforms
if (WIN32)
	add_executable(VrTest WIN32 ${SOURCE_FILES})
else()
	add_executable(VrTest ${SOURCE_FILES})
endif()

target_link_libraries(VrTest PRIVATE SDL2main)
target_link_libraries(VrTest PRIVATE SDL2-static)
target_link_libraries(VrTest PRIVATE glad)
target_link_libraries(VrTest PRIVATE imgui)

target_compile_options(SDL2main PRIVATE /w)
target_compile_options(SDL2-static PRIVATE /w)
target_compile_options(glad PRIVATE /w)
target_compile_options(imgui PRIVATE /w)

# todo: add all the different openvr versions for each platform
target_link_libraries(VrTest PRIVATE openvr_api64)
target_link_libraries(VrTest PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/openvr/lib/win64/openvr_api.lib" )
add_custom_command(TARGET VrTest POST_BUILD # copy dll
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/thirdparty/openvr/bin/win64/openvr_api.dll"
		$<TARGET_FILE_DIR:VrTest>)

# copy core assets
add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/core"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/core" $<TARGET_FILE_DIR:VrTest>/core
	MAIN_DEPENDENCY "${CMAKE_SOURCE_DIR}/core"
)
add_custom_target(
	CopyCore ALL
	DEPENDS "${CMAKE_BINARY_DIR}/core"
)
add_dependencies(VrTest CopyCore)

# include src
target_include_directories(VrTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# high warning level
target_compile_options(VrTest PRIVATE /W4 /WX)

# precompiled header
target_precompile_headers(VrTest PRIVATE "pch.h")

# version and stuff in a config file
configure_file(VrTestConfig.h.in VrTestConfig.h)