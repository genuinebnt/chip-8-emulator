cmake_minimum_required(VERSION 3.22)
project(chip8_Emulator)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(chip8_Emulator src/main.cpp src/Chip8.cpp src/Chip8.h src/Platform.cpp src/Platform.h)
target_link_libraries(chip8_Emulator ${CONAN_LIBS})
