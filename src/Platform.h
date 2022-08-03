//
// Created by genui on 8/3/2022.
//

#ifndef CHIP8_EMULATOR_PLATFORM_H
#define CHIP8_EMULATOR_PLATFORM_H

#include <SDL.h>
#include <iostream>
using std::cout;

class Platform {
private:
    SDL_Window *_window{};
    SDL_Renderer *_renderer{};
    SDL_Texture *_texture{};

public:
    Platform() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            throw std::exception(SDL_GetError());
        }
    }
    ~Platform() {
        SDL_Quit();
    }
};


#endif//CHIP8_EMULATOR_PLATFORM_H
