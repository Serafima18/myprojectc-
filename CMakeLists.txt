cmake_minimum_required(VERSION 3.29)

project(checkers)

set(TARGET "checkers")

set(HEADERS
    src/board.h
    src/cell.h
    src/game_logic.h
    src/piece.h
)

set(SOURCES
    src/board.cpp
    src/game_logic.cpp
    src/main.cpp
    src/piece.cpp
)

project(${TARGET} CXX)

set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FORMS True)

find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${FLTK_INCLUDE_DIR})
link_directories(${FLTK_INCLUDE_DIR}/../fltk)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#   target_link_libraries(${TARGET} fltk_jpeg fltk_png fltk_z)
# endif()

install(TARGETS ${TARGET})
