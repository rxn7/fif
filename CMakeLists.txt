cmake_minimum_required(VERSION 3.15)
project(fif)

option(FIF_BUILD_EXAMPLE "Build the example project" ON)
option(FIF_BUILD_CORE "Build the core module" ON)
option(FIF_BUILD_GFX "Build the gfx module" ON)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(-fPIC)

if(FIF_BUILD_CORE)
  FILE(GLOB_RECURSE CoreSources ${SRC_DIR}/core/*.cpp)
  add_library(fif-core SHARED ${CoreSources})

  target_include_directories(fif-core PUBLIC ${INC_DIR} PRIVATE ${SRC_DIR}/core)
  target_compile_definitions(fif-core PRIVATE GLFW_INCLUDE_NONE)
  target_link_libraries(fif-core PRIVATE glfw ${GLFW_LIBRARIES})
  target_link_libraries(fif-core PRIVATE glad)
  target_link_libraries(fif-core PRIVATE EnTT::EnTT)
endif()

if(FIF_BUILD_GFX)
  FILE(GLOB_RECURSE GfxSources ${SRC_DIR}/gfx/*.cpp)
  add_library(fif-gfx SHARED ${GfxSources})

  target_include_directories(fif-gfx PUBLIC ${INC_DIR} PRIVATE ${SRC_DIR}/gfx)
  target_link_libraries(fif-gfx PUBLIC fif-core)
  target_link_libraries(fif-gfx PRIVATE glad)
endif()

if(FIF_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")

add_subdirectory(${LIB_DIR}/glfw)
add_subdirectory(${LIB_DIR}/entt)
add_subdirectory(${LIB_DIR}/glad)
include_directories(${LIB_DIR}/glm)
