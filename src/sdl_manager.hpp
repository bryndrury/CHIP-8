// SDL MANAGER CLASS

#ifndef SDL_MANAGER_HPP
#define SDL_MANAGER_HPP

#include <iostream>
#include <cmath>

#include "SDL.h"

class SDLManager
{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Uint32 frameTime;

    SDLManager();
    ~SDLManager();

    void passDisplayBuffer(const std::array<uint8_t, 64 * 32> &buffer);
    void renderDisplayBuffer();
    void playBeep();

private:
    bool init();
    void quit();

    std::array<uint8_t, 64 * 32> display_buffer;    // Graphics buffer
};

#endif // SDL_MANAGER_HPP