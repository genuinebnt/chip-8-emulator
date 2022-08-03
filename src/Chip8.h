//
// Created by genui on 7/29/2022.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

using std::cerr;
using std::cin;
using std::cout;
using std::string;
namespace fs = std::filesystem;
const unsigned int START_ADDRESS = 0x200;
const unsigned int SCREEN_WIDTH{64};
const unsigned int SCREEN_HEIGHT{32};
const unsigned int FONT_SET_SIZE{80};
const unsigned int FONT_SET_START_ADDRESS{0x50};

class Chip8 {
private:
    unsigned short _opcode{};
    unsigned char _memory[4096]{};
    unsigned char _registers[16]{};
    unsigned short _index{};
    unsigned short _pc{};
    unsigned int _screen[SCREEN_WIDTH * SCREEN_HEIGHT]{};
    unsigned char _delay_timer{};
    unsigned char _sound_timer{};
    unsigned short _stack[16]{};
    unsigned short _sp{};
    unsigned char _keyboard[16]{};
    std::default_random_engine _rand_gen;
    std::uniform_int_distribution<std::mt19937::result_type> _rand_byte;

public:
    Chip8();

    void LoadRom(const fs::path &path);

    void Cycle();

    void OP_00E0();

    void OP_00EE();

    void OP_1NNN();

    void OP_2NNN();

    void OP_3XNN();

    void OP_4XNN();

    void OP_5XY0();

    void OP_6XNN();

    void OP_7XNN();

    void OP_8XY0();

    void OP_8XY1();

    void OP_8XY2();

    void OP_8XY3();

    void OP_8XY4();

    void OP_8XY5();

    void OP_8XY6();

    void OP_8XY7();

    void OP_8XYE();

    void OP_9XY0();

    void OP_ANNN();

    void OP_BNNN();

    void OP_CXNN();

    void OP_DXYN();

    void OP_EX9E();

    void OP_EXA1();

    void OP_FX07();

    void OP_FX0A();

    void OP_FX15();

    void OP_FX18();

    void OP_FX1E();

    void OP_FX29();

    void OP_FX33();

    void OP_FX55();

    void OP_FX65();
};

#endif//CHIP8_EMULATOR_CHIP8_H
