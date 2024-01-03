// Using SDL and standard IO
#include <SDL2\SDL.h>
#include <stdio.h>
#include <stdbool.h>

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// The image we will load and show on the screen
SDL_Surface *gXOut = NULL;
// Loads individual image
SDL_Surface *loadSurface(const char *path); // Remove the "std::" namespace qualifier
// Current displayed image
SDL_Surface *gCurrentSurface = NULL;
// The window renderer
SDL_Renderer *renderer = NULL;

// Init Functions
void init();
SDL_Surface *loadMedia(char *path, int scale);
void renderMap(SDL_Surface *map, int x, int y);
bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map);
void close();

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
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

SDL_Surface *loadMedia(char *path, int scale)
{
    // Load splash image
    SDL_Surface *originalSurface = SDL_LoadBMP(path);
    if (originalSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        close();
    }

    int desiredWidth = originalSurface->h * scale;
    int desiredHeight = originalSurface->h * scale;

    // Scale the surface to the desired size
    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(0, desiredWidth, desiredHeight, originalSurface->format->BitsPerPixel,
                                                      originalSurface->format->Rmask, originalSurface->format->Gmask, originalSurface->format->Bmask, originalSurface->format->Amask);

    SDL_BlitScaled(originalSurface, NULL, scaledSurface, NULL);

    // Free the original surface as it is no longer needed
    SDL_FreeSurface(originalSurface);

    return scaledSurface;
}

// Function to render a map at a specific position
void renderMap(SDL_Surface *map, int x, int y)
{
    SDL_Rect destRect = {x, y, 0, 0};
    SDL_BlitSurface(map, NULL, gScreenSurface, &destRect);
}

// Function to check if the mouse is inside a given rectangle
bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map)
{
    int screen_height = gScreenSurface->h;
    int map_height = map->h;

    return mouseX >= x_start && mouseX <= x_start + map->w && mouseY >= (screen_height - map_height) / 2 && mouseY <= (screen_height - map_height) / 2 + map_height;
}

void close()
{
    // Deallocate surface
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}
