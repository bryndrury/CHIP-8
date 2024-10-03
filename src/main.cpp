#include <iostream>

#include "SDL.h"

#include "chip8.hpp"
#include "sdl_manager.hpp"

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    SDLManager sdl;
    Chip8 chip8;

    while (chip8.currentState == Chip8::State::RUNNING)
    {
        while (SDL_PollEvent(&sdl.event))
        {
            if (sdl.event.type == SDL_QUIT)
            {
                chip8.currentState = Chip8::State::STOPPED;
            }
        }

        chip8.EmulateCycle();

        if (chip8.drawFlag)
        {
            sdl.passDisplayBuffer(chip8.display_buffer);
            sdl.renderDisplayBuffer();
            SDL_RenderPresent(sdl.renderer);

            chip8.drawFlag = false;
        }

        SDL_RenderPresent(sdl.renderer);

        SDL_Delay(16);
    }
    
    return(EXIT_SUCCESS);
}
