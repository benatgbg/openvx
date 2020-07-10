cmake_minimum_required(VERSION 2.8.9)

set(THIRDPARTY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
set(PROJECT_NAME external_openvx_test)
project(${PROJECT_NAME})

# openvx
include(ExternalProject)
ExternalProject_Add(openvx
  GIT_REPOSITORY    https://github.com/KhronosGroup/OpenVX-sample-impl.git
  GIT_TAG           openvx_1.3
  UPDATE_COMMAND    ""
  INSTALL_DIR       ${THIRDPARTY_ROOT}
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
add_library(openvx::vx STATIC IMPORTED)
add_library(openvx::vxu STATIC IMPORTED)
add_library(openvx::vx_debug STATIC IMPORTED)
add_library(openvx::vx_debug_lib STATIC IMPORTED)
add_library(openvx::helper STATIC IMPORTED)
if(WIN32)
    set_target_properties(openvx::vx PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_ROOT}/lib/openvx.lib)
    set_target_properties(openvx::vxu PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_ROOT}/lib/vxu.lib)
    set_target_properties(openvx::vx_debug PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_ROOT}/bin/vx_debug.lib)
    set_target_properties(openvx::vx_debug_lib PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_ROOT}/bin/openvx-debug-lib.lib)
    set_target_properties(openvx::helper PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_ROOT}/bin/openvx-helper.lib)
endif()
include_directories(SYSTEM ${THIRDPARTY_ROOT}/include)

# sample app
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} openvx::vx openvx::vxu openvx::vx_debug openvx::vx_debug_lib openvx::helper)
add_dependencies(${PROJECT_NAME} openvx)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/colorbars_640x480_I444.rgb $<TARGET_FILE_DIR:${PROJECT_NAME}>)