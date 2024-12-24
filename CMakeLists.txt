cmake_minimum_required(VERSION 3.29)

set(TARGET "checkers")

set(HEADERS
    src/board.h
    src/game.h
    src/piece.h
)
set(SOURCES
    src/board.cpp
    src/game.cpp
    src/main.cpp
    src/piece.cpp
)

project(${TARGET} CXX)

set(CMAKE_CXX_STANDARD 11)
set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FORMS True)

find_package(FLTK 1.3.8 EXACT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${FLTK})
link_directories(${FLTK_INCLUDE_DIR}/../lib)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} ${FLTK})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#[[    target_link_libraries(${TARGET} fltk_jpeg fltk_png fltk_z)]]
endif()

install(TARGETS ${TARGET})