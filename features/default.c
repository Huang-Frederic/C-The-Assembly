#include "../headers/include.h"
#include "../headers/default.h"

void init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        close();
    }
    else if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        close();
    }
    else if (!loadFont(selectedFont, 24))
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        close();
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("Slay The Assembly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            close();
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
}

bool loadFont(const char *selectedFont, int font_size)
{
    font = TTF_OpenFont(selectedFont, font_size);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void close()
{
    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}