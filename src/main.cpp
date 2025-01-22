#include <iostream>

#include "SDL.h"

#include "chip8.hpp"
#include "sdl_manager.hpp"

#define CLOCK_SPEED 60

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    SDLManager sdl;
    Chip8 chip8;

    while (chip8.currentState == Chip8::State::RUNNING)
    {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&sdl.event))
        {
            if (sdl.event.type == SDL_QUIT)
            {
                chip8.currentState = Chip8::State::STOPPED;
            }
            switch (sdl.event.type)
            {
            case SDL_KEYDOWN:
                std::cout << "Keydown detected!!!\n"; 
            default:
                break;
            }
        }

        chip8.EmulateCycle();

        // Display
        if (chip8.drawFlag)
        {
            sdl.passDisplayBuffer(chip8.display_buffer);
            sdl.renderDisplayBuffer();
            SDL_RenderPresent(sdl.renderer);

            chip8.drawFlag = false;
        }

        // Sound
        if (chip8.soundFlag)
        {
            // Play beep sound via SDL
            sdl.playBeep();
            std::cout << "Beep\n";
            chip8.soundFlag = false;
        }

        SDL_RenderPresent(sdl.renderer);

        sdl.frameTime = SDL_GetTicks() - frameStart;
        if (sdl.frameTime < (1000 / CLOCK_SPEED))
        {
            SDL_Delay( (1000 / CLOCK_SPEED) - sdl.frameTime );
        }
    }
    
    return(EXIT_SUCCESS);
}
