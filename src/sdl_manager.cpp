#include "sdl_manager.hpp"

// class SDLManager
// {
// public:
//     SDL_Window* window;

//     SDLManager();
//     ~SDLManager();

//     bool init();
//     void quit();
// };

SDLManager::SDLManager()
{
    if (init() == false)
    {
        std::cerr << "SDLManager init failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

SDLManager::~SDLManager()
{
    quit();
}

bool SDLManager::init()
{
    if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO| SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // WINDOW
    // ===============================
    window = nullptr;
    window = SDL_CreateWindow("CHIP-8",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640, 320,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    // ===============================

    // RENDERER
    // ===============================
    renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    
    return true;
}

void SDLManager::quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLManager::passDisplayBuffer(const std::array<uint8_t, 64 * 32> &buffer)
{
    display_buffer = buffer;
}

void SDLManager::renderDisplayBuffer()
{
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 64 * 32; i++)
    {
        int x, y;
        x = i % 64;
        y = i / 64;

        if (display_buffer[i] == 0)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 50, 220, 75, 255);
        }

        SDL_Rect rect = {x * 10, y * 10, 10, 10};
        SDL_RenderFillRect(renderer, &rect);
    }
}