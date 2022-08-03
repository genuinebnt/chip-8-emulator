#include "Chip8.h"
#include "Platform.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
    Chip8 chip8;
    chip8.LoadRom("../test_opcode.ch8");

    try {
        Platform platform;
    } catch (std::exception& e) {
        cout << "Error: " << e << '\n';
    }

    return 0;
}
