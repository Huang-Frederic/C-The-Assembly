// Using SDL and standard IO
#include <SDL2\SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "headers/map.h"

int main(int argc, char *args[])
{
    bool quit = false;
    SDL_Event e;

    // Initialize SDL and load media
    if (!init() || !loadMedia())
    {
        printf("Failed to initialize or load media!\n");
        return 1; // Return an error code
    }

    // Set default current surface
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
}