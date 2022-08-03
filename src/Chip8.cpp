//
// Created by genui on 7/29/2022.
//

#include "Chip8.h"

Chip8::Chip8() : _rand_gen(std::chrono::system_clock::now().time_since_epoch().count()), _pc(START_ADDRESS), _index(0),
                 _sp(0) {
    _rand_byte = std::uniform_int_distribution<std::mt19937::result_type>(0, 255U);


    unsigned char font_set[FONT_SET_SIZE]{
            0xF0, 0x90, 0x90, 0x90, 0xF0,// 0
            0x20, 0x60, 0x20, 0x20, 0x70,// 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0,// 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0,// 3
            0x90, 0x90, 0xF0, 0x10, 0x10,// 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0,// 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0,// 6
            0xF0, 0x10, 0x20, 0x40, 0x40,// 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0,// 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0,// 9
            0xF0, 0x90, 0xF0, 0x90, 0x90,// A
            0xE0, 0x90, 0xE0, 0x90, 0xE0,// B
            0xF0, 0x80, 0x80, 0x80, 0xF0,// C
            0xE0, 0x90, 0x90, 0x90, 0xE0,// D
            0xF0, 0x80, 0xF0, 0x80, 0xF0,// E
            0xF0, 0x80, 0xF0, 0x80, 0x80 // F
    };

    for (unsigned int i = 0; i < FONT_SET_SIZE; ++i) {
        _memory[FONT_SET_START_ADDRESS + i] = font_set[i];
    }
}

void Chip8::LoadRom(const fs::path &path) {
    std::fstream file(path, std::ios::in | std::ios::binary);

    if (file.is_open()) {
        const auto fz = file.tellg();

        file.read(reinterpret_cast<char *>(&_memory[START_ADDRESS]), fz);
        file.close();
    }
}

void Chip8::Cycle() {
    _opcode = _memory[_pc] << 8u | _memory[_pc + 1];

    _pc += 2;

    switch (_opcode & 0xF000u) {
        case 0x0000u:
            switch (_opcode & 0x000Fu) {
                case 0x0000u:
                    OP_00E0();
                    break;
                case 0x000Eu:
                    OP_00EE();
                    break;
            }
            break;
        case 0x1000u:
            OP_1NNN();
            break;
        case 0x2000u:
            OP_2NNN();
            break;
        case 0x3000u:
            OP_3XNN();
            break;
        case 0x4000u:
            OP_4XNN();
            break;
        case 0x5000u:
            OP_5XY0();
            break;
        case 0x6000u:
            OP_6XNN();
            break;
        case 0x7000u:
            OP_7XNN();
            break;
        case 0x8000u:
            switch (_opcode & 0x000Fu) {
                case 0x0000u:
                    OP_8XY0();
                    break;
                case 0x0001u:
                    OP_8XY1();
                    break;
                case 0x0002u:
                    OP_8XY2();
                    break;
                case 0x0003u:
                    OP_8XY3();
                    break;
                case 0x0004u:
                    OP_8XY4();
                    break;
                case 0x0005u:
                    OP_8XY5();
                    break;
                case 0x0006u:
                    OP_8XY6();
                    break;
                case 0x0007u:
                    OP_8XY7();
                    break;
                case 0x000Eu:
                    OP_8XYE();
                    break;
            }
            break;
        case 0x9000u:
            OP_9XY0();
            break;
        case 0xA000u:
            OP_ANNN();
            break;
        case 0xB000u:
            OP_BNNN();
            break;
        case 0xC000u:
            OP_CXNN();
            break;
        case 0xD000u:
            OP_DXYN();
            break;
        case 0xE000u:
            switch (_opcode & 0x00FFu) {
                case 0x009Eu:
                    OP_EX9E();
                    break;
                case 0x00A1:
                    OP_EXA1();
                    break;
            }
            break;
        case 0xF000u:
            switch (_opcode & 0x00FFu) {
                case 0x0007u:
                    OP_FX07();
                    break;
                case 0x000Au:
                    OP_FX0A();
                    break;
                case 0x0015u:
                    OP_FX15();
                    break;
                case 0x0018u:
                    OP_FX18();
                    break;
                case 0x001Eu:
                    OP_FX1E();
                    break;
                case 0x0029:
                    OP_FX29();
                    break;
                case 0x0033:
                    OP_FX33();
                    break;
                case 0x0055:
                    OP_FX55();
                    break;
                case 0x0065:
                    OP_FX65();
                    break;
            }
            break;
    }

    if (_delay_timer > 0)
        _delay_timer -= 1;
    if (_sound_timer > 0)
        _sound_timer -= 1;
}

void Chip8::OP_00E0() {
    //    for(unsigned char & i : _screen) {
    //        i ^= i;
    //    }
    memset(_screen, 0, sizeof(_screen));
}

void Chip8::OP_00EE() {
    _sp--;
    _pc = _stack[_sp];
}

void Chip8::OP_1NNN() {
    _pc = _opcode & 0x0FFFu;
}

void Chip8::OP_2NNN() {
    _stack[_sp] = _pc;
    ++_sp;
    _pc = _opcode & 0x0FFFu;
}

void Chip8::OP_3XNN() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    if (_registers[x] == (_opcode & 0x00FFu)) {
        _pc += 2;
    }
}

void Chip8::OP_4XNN() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    if (_registers[x] != (_opcode & 0x00FFu)) {
        _pc += 2;
    }
}

void Chip8::OP_5XY0() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    unsigned char y = (_opcode & 0x00F0u) >> 4u;
    if (_registers[x] == _registers[y]) {
        _pc += 2;
    }
}

void Chip8::OP_6XNN() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    _registers[x] = _opcode & 0x00FFu;
}

void Chip8::OP_7XNN() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    _registers[x] += _opcode & 0x00FFu;
}

void Chip8::OP_8XY0() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    _registers[x] = _registers[y];
}

void Chip8::OP_8XY1() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    _registers[x] |= _registers[y];
}

void Chip8::OP_8XY2() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    _registers[x] &= _registers[y];
}

void Chip8::OP_8XY3() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    _registers[x] ^= _registers[y];
}

void Chip8::OP_8XY4() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    unsigned short sum = _registers[x] + _registers[y];

    if (sum > 255u) {
        _registers[0xF] = 1;
    } else {
        _registers[0xF] = 0;
    }

    _registers[x] = sum & 0x00FFu;
}

void Chip8::OP_8XY5() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;

    if (_registers[x] > _registers[y]) {
        _registers[0xF] = 1;
    } else {
        _registers[0xF] = 0;
    }

    _registers[x] -= _registers[y];
}

void Chip8::OP_8XY6() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    _registers[0xF] = _registers[x] & 0x01u;
    _registers[x] >>= 1;
}

void Chip8::OP_8XY7() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    if (_registers[y] < _registers[x]) {
        _registers[0xF] = 0;
    } else {
        _registers[0xF] = 1;
    }
    _registers[x] = _registers[y] - _registers[x];
}

void Chip8::OP_8XYE() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    _registers[0xF] = (_registers[x] & 0x80u) >> 7u;
    _registers[x] <<= 1;
}

void Chip8::OP_9XY0() {
    unsigned char x = (_opcode & 0X0F00u) >> 8u;
    unsigned char y = (_opcode & 0X00F0u) >> 4u;
    if (_registers[x] != _registers[y]) {
        _pc += 2;
    }
}

void Chip8::OP_ANNN() {
    _index = (_opcode & 0x0FFFu);
}

void Chip8::OP_BNNN() {
    unsigned short address = _opcode & 0x0FFFu;
    _pc = address + _registers[0];
}

void Chip8::OP_CXNN() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    unsigned short address = _opcode & 0x00FFu;
    _registers[x] = address & _rand_byte(_rand_gen);
}

void Chip8::OP_DXYN() {
    unsigned char x_cor = _registers[(_opcode & 0x0F00u) >> 8u] % SCREEN_WIDTH;
    unsigned char y_cor = _registers[(_opcode & 0x00F0u) >> 4u] % SCREEN_HEIGHT;
    unsigned short height = _opcode & 0x000Fu;
    _registers[0xF] = 0;

    for (unsigned int i = 0; i < height; ++i) {
        auto sprite_byte = _memory[i + _index];
        for (unsigned int j = 0; j < 8; ++j) {
            unsigned char sprite_pixel = sprite_byte & (0x80u >> j);
            unsigned int screen_pixel = _screen[(y_cor + j) * SCREEN_WIDTH + (x_cor + i)];
            if (sprite_pixel) {
                if (screen_pixel == 0xFFFFFFFF) {
                    _registers[0xF] = 1;
                }
            }
            screen_pixel ^= sprite_pixel;
        }
    }
}

void Chip8::OP_EX9E() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    auto key = _registers[x];
    if (_keyboard[key]) {
        _pc += 2;
    }
}

void Chip8::OP_EXA1() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    auto key = _registers[x];
    if (!_keyboard[key]) {
        _pc += 2;
    }
}

void Chip8::OP_FX07() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    _registers[x] = _delay_timer;
}

void Chip8::OP_FX0A() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    while (true) {
        for (unsigned int i = 0; i < 16; ++i) {
            if (_keyboard[i]) {
                _registers[x] = i;
                return;
            }
        }
    }
}

void Chip8::OP_FX15() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    _registers[x] = _delay_timer;
}

void Chip8::OP_FX18() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    _registers[x] = _sound_timer;
}

void Chip8::OP_FX1E() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    _index += _registers[x];
}

void Chip8::OP_FX29() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    auto character = _registers[x];
    _index = FONT_SET_START_ADDRESS + (character * 5);
}

void Chip8::OP_FX33() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    auto val = _registers[x];

    _memory[_index] = val / 100;
    val %= 100;
    _memory[_index + 1] = val / 10;
    val %= 10;
    _memory[_index + 2] = val / 1;
}

void Chip8::OP_FX55() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    for (unsigned int i = 0; i <= x; ++i) {
        _memory[_index + i] = _registers[i];
    }
}

void Chip8::OP_FX65() {
    unsigned char x = (_opcode & 0x0F00u) >> 8u;
    for (unsigned int i = 0; i <= x; ++i) {
        _registers[i] = _memory[_index + i];
    }
}
