cmake_minimum_required(VERSION 3.26)
project(fif)

option(FIF_BUILD_EDITOR "Build the editor (fifed)" ON)
option(FIF_BUILD_CORE "Build the core module" ON)
option(FIF_BUILD_GFX "Build the gfx module" ON)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-fPIC)

if(FIF_BUILD_CORE)
  FILE(GLOB_RECURSE FIF_CORE_SOURCES ${SRC_DIR}/core/*.cpp)
  add_library(fif-core SHARED ${FIF_CORE_SOURCES})

  set_target_properties(fif-core PROPERTIES PUBLIC_HEADER ${INC_DIR}/gfx)
  target_compile_definitions(fif-core PRIVATE GLFW_INCLUDE_NONE)
  target_include_directories(fif-core PUBLIC ${INC_DIR} PRIVATE ${SRC_DIR}/core)
  target_link_libraries(fif-core PRIVATE glfw glad EnTT::EnTT ImGui)
endif()

if(FIF_BUILD_GFX)
  FILE(GLOB_RECURSE FIF_GFX_SOURCES ${SRC_DIR}/gfx/*.cpp)
  add_library(fif-gfx SHARED ${FIF_GFX_SOURCES})

  set_target_properties(fif-gfx PROPERTIES PUBLIC_HEADER ${INC_DIR}/gfx)
  target_include_directories(fif-gfx PUBLIC ${INC_DIR}/gfx PRIVATE ${SRC_DIR}/gfx)
  target_link_libraries(fif-gfx PUBLIC fif-core glad)
endif()

if(FIF_BUILD_EDITOR)
  add_subdirectory(editor)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")

add_subdirectory(${LIB_DIR}/glfw)
add_subdirectory(${LIB_DIR}/entt)
add_subdirectory(${LIB_DIR}/glad)
include_directories(${LIB_DIR}/glm)

# ImGui
set(IMGUI_PATH "vendor/imgui")

file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp
)

add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
