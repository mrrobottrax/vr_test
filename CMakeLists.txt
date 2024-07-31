cmake_minimum_required(VERSION 3.27.2)

project(VrTestParent VERSION 0.0)

add_subdirectory(thirdparty)
add_subdirectory(src)

set(VS_STARTUP_PROJECT VrTest)