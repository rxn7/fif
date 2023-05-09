set(TINYFD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinyfd)

add_library("tinyfd" STATIC ${TINYFD_PATH}/tinyfiledialogs.c)
target_include_directories("tinyfd" PUBLIC ${TINYFD_PATH})
