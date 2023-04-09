cmake_minimum_required(VERSION 3.25)
project(weiyi C)

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

message(STATUS "OPENGL_FOUND = ${OPENGL_FOUND}")
message(STATUS "OPENGL_XMESA_FOUND = ${OPENGL_XMESA_FOUND}")
message(STATUS "OPENGL_GLU_FOUND = ${OPENGL_GLU_FOUND}")
message(STATUS "OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-lX11 -lm")
FILE(COPY word.txt DESTINATION "${CMAKE_BINARY_DIR}")
#link_directories(${X11_LIBRARIES})

add_executable(weiyi main.c gfx.c gfx.h gfx.c gfx.h)
