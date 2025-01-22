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

void SDLManager::playBeep()
{
    const int amplitude = 28000;    // Max amplitude
    const int frequency = 440;      // A4
    const int sample_rate = 44100;  // 44.1kHz
    const int duration = 16.66;     // 1/60th = 16.66ms (60Hz)

    SDL_AudioSpec audio_spec;       // 
    Uint32 wav_length;              // 

    // Generate the sine wave sample
    audio_spec.freq = sample_rate;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 2048;
    audio_spec.callback = nullptr;

    int sample_count = (sample_rate * duration) / 1000;
    std::array<Uint8, (44100 * 16) / 1000 * sizeof(Sint16)> wav_buffer;
    for (int i = 0; i < sample_count; i++)
    {
        Sint16 sample = (Sint16)(amplitude * sin(2 * M_PI * frequency * i / sample_rate));
        wav_buffer[i * sizeof(Sint16)] = sample & 0xFF;
        wav_buffer[i * sizeof(Sint16) + 1] = (sample >> 8) & 0xFF;
    }
    wav_length = sample_count * sizeof(Sint16);

    if (SDL_OpenAudio(&audio_spec, nullptr) < 0)
    {
        std::cerr << "SDL_OpenAudio Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_QueueAudio(1, wav_buffer.data(), wav_length);
    SDL_PauseAudio(0);

    // SDL_Delay(duration);

    SDL_CloseAudio();
}