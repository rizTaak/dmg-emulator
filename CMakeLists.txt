cmake_minimum_required(VERSION 3.10)
project(dmg VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(SDL2 REQUIRED)
#find_package(X11 REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)

#include_directories(${X11_INCLUDE_DIR})
#include_directories(${OPENGL_INCLUDE_DIR})
#include_directories(${GLUT_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(include)
include_directories(lib)

configure_file(include/version.h.in version.h)
file(COPY roms DESTINATION ${CMAKE_BINARY_DIR})

set(EMU_CPP_FILES ${PROJECT_SOURCE_DIR}/src/emulator/rom.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/cpu.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_0x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_1x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_2x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_3x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_4x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_5x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_6x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_7x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_8x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_9x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_ax.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_bx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_cx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_dx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_ex.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/ops_fx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_fx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_0x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_1x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_2x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_3x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_4x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_5x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_6x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_7x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_8x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_9x.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_ax.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_bx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_cx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_dx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_ex.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/cb/ops_fx.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/ops/common.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/dmg.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/op.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/memory.cpp
        ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/gpu.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/timer.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/common.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/joypad.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/cartridge.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/sdl2.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/time.cpp
        ${PROJECT_SOURCE_DIR}/src/emulator/apu.cpp)

#link_directories(${X11_LIBRARIES})

add_executable(${PROJECT_NAME} ${EMU_CPP_FILES} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
