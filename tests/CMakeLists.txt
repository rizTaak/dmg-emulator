project(gbtests)

find_package(SDL2 REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${X11_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

configure_file(../roms/dmg_rom.bin ${CMAKE_CURRENT_BINARY_DIR}/dmg_rom.bin COPYONLY)
configure_file(../roms/games/tetris.gb ${CMAKE_CURRENT_BINARY_DIR}/tetris.gb COPYONLY)

set(CPP_FILES common.cpp
        basic.cpp)
link_directories(${X11_LIBRARIES})
add_executable(${PROJECT_NAME} ${CPP_FILES} ${EMU_CPP_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARIES})