cmake_minimum_required(VERSION 3.15)
project(fif DESCRIPTION "Core of the fif engine")

option(FIF_BUILD_EXAMPLE "Builds the example project" ON)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(-fPIC)

FILE(GLOB Sources ${SRC_DIR}/*.cpp)
add_library(${PROJECT_NAME} SHARED ${Sources})
include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR} PRIVATE ${SRC_DIR})

# Link GLFW
set(GLFW_DIR ${LIB_DIR}/glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW docs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Install GLFW")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# Link GLAD
set(GLAD_DIR ${LIB_DIR}/glad)
add_subdirectory(${GLAD_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# Link GLM
set(GLM_DIR ${LIB_DIR}/glm)
include_directories(${GLM_DIR})

if (FIF_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()
