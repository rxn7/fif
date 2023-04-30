set(LUAJIT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor/LuaJIT)

file(GLOB_RECURSE LUAJIT_SOURCES ${LUAJIT_PATH}/*.cpp)

add_library("LuaJIT" STATIC ${IMGUI_SOURCES})
target_include_directories("LuaJIT" PUBLIC ${LUAJIT_PATH}/src)
set_target_properties("LuaJIT" PROPERTIES LINK_FLAGS "${LFLAGS}")
