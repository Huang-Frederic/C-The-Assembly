// // Using SDL and standard IO
// #include <SDL2\SDL.h>
// #include <SDL2\SDL_image.h>
// #include <stdio.h>
// #include <stdbool.h>

#include "headers/map.h"

int main(int argc, char *args[])
{
    // Start up SDL and create window
    init();

    // Load media
    SDL_Surface *map_camp = loadMedia("camp.png", 5);
    SDL_Surface *map_fight = loadMedia("fight.png", 5);
    SDL_Surface *map_treasure = loadMedia("treasure.png", 5);

    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;

    // Calculate the x-coordinate for centering each map
    int screenWidth = gScreenSurface->w;
    int mapWidth = map_camp->w; // Assuming all maps have the same width

    int totalMapsWidth = 3 * mapWidth + 2 * 40; // 40 pixels spacing between maps
    int x_start = (screenWidth - totalMapsWidth) / 2;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            if (e.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                map_camp = isMouseInside(x, y, x_start, map_camp) ? loadMedia("camp.png", 6) : loadMedia("camp.png", 5);
                isMouseInside(x, y, x_start, map_camp) ? printf("Camp\n") : 0;
                map_fight = isMouseInside(x, y, x_start + mapWidth + 40, map_fight) ? loadMedia("fight.png", 6) : loadMedia("fight.png", 5);
                isMouseInside(x, y, x_start + mapWidth + 40, map_fight) ? printf("Fight\n") : 0;
                map_treasure = isMouseInside(x, y, x_start + 2 * (mapWidth + 40), map_treasure) ? loadMedia("treasure.png", 6) : loadMedia("treasure.png", 5);
                isMouseInside(x, y, x_start + 2 * (mapWidth + 40), map_treasure) ? printf("Treasure\n") : 0;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (SDL_BUTTON_LEFT == e.button.button)
                {
                    printf("Left mouse button is down\n");
                    quit = true;
                }
            }
        }

        // Clear the screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

        // Render maps at centered positions
        renderMap(map_camp, x_start, (gScreenSurface->h - map_camp->h) / 2);
        renderMap(map_fight, x_start + mapWidth + 40, (gScreenSurface->h - map_fight->h) / 2);
        renderMap(map_treasure, x_start + 2 * (mapWidth + 40), (gScreenSurface->h - map_treasure->h) / 2);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }

    // Free resources and close SDL
    close();

    return 0;
}
