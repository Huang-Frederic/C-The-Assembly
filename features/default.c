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
        close_SDL();
    }
    else if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        close_SDL();
    }
    else if (!loadFont(selectedFont, 24))
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        close_SDL();
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            close_SDL();
        }
        else
        {
            Renderer = SDL_CreateRenderer(gWindow, -1, 0);
            if (!Renderer)
            {   printf("\nError Renderer : %s", SDL_GetError());
                printf("\nThere was a problem creating the renderer.");
                close_SDL();
            }
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            SDL_Surface *icon = IMG_Load("data/Logo.png");
            SDL_SetWindowIcon(gWindow, icon);
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

void close_SDL()
{
    SDL_DestroyRenderer(Renderer);
    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}
