// // Using SDL and standard IO
// #include <SDL2\SDL.h>
// #include <SDL2\SDL_image.h>
// #include <stdio.h>
// #include <stdbool.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include <ctype.h>

#include "headers/map.h"

int main(int argc, char *args[])
{
    // Start up SDL and create window
    init();

    //////////////////////////////////////////////
    // Load background
    SDL_Surface *background = loadMedia("background", 1);
    //////////////////////////////////////////////

    char map_choices[5][20] = {
        "Common",
        "Elite",
        "Treasure",
        "Camp",
        "Boss",
    };
    char countries[5][20] = {"France", "Germany", "China", "Korea", "Japan"};

    srand(time(NULL));

    for (int z = 0; z < 100; z++)
    {
        printf("Iterative: %d\n", z + 1);
        int map_occurence = rand() % 2 + 2;

        char map_chosen[3][20];

        printf("map_occurence: %d\n", map_occurence);

        for (int i = 0; i < map_occurence; i++)
        {
            int randomNumber = rand() % 10;

            if ((z + 1) % 10 == 0)
            {
                randomNumber = 10;
                map_occurence = 1;
            }

            switch (randomNumber)
            {
            case 0 ... 4:
                strcpy(map_chosen[i], map_choices[0]);
                strcat(map_chosen[i], countries[rand() % 5]);
                break;
            case 5 ... 6:
                strcpy(map_chosen[i], map_choices[1]);
                strcat(map_chosen[i], countries[rand() % 5]);
                break;
            case 7:
                strcpy(map_chosen[i], map_choices[2]);
                break;
            case 8 ... 9:
                strcpy(map_chosen[i], map_choices[3]);
                break;
            case 10:
                strcpy(map_chosen[i], map_choices[4]);
                strcat(map_chosen[i], countries[rand() % 5]);
                break;
            }

            for (int j = 0; j < i; j++)
            {
                if (strcmp(map_chosen[i], map_chosen[j]) == 0)
                {
                    i--;
                }
            }
        }

        for (int i = 0; i < map_occurence; i++)
        {
            printf("%s\n", map_chosen[i]);
        }

        // Load media
        SDL_Surface *maps[map_occurence];

        for (int i = 0; i < map_occurence; i++)
        {
            maps[i] = loadMedia(map_chosen[i], 5);
        }

        // Main loop flag
        bool quit = false;
        // Event handler
        SDL_Event e;

        // Calculate the x-coordinate for centering each map
        int screenWidth = gScreenSurface->w;
        int totalMapsWidth = 0;

        // Assuming all maps have the same width
        int mapWidth = maps[0]->w;
        int spacing = 40; // 40 pixels spacing between maps

        for (int i = 0; i < map_occurence; i++)
        {
            totalMapsWidth += mapWidth + spacing;
        }

        int x_start = (screenWidth - totalMapsWidth + spacing) / 2;

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

                    for (int i = 0; i < map_occurence; i++)
                    {
                        maps[i] = isMouseInside(x, y, x_start + i * (mapWidth + 40), maps[i]) ? loadMedia(map_chosen[i], 6) : loadMedia(map_chosen[i], 5);
                        isMouseInside(x, y, x_start + i * (mapWidth + 40), maps[i]) ? printf("%s\n", map_chosen[i]) : 0;
                    }
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

            //////////////////////////////////////////////
            // Render background
            renderMap(background, 0, 0);
            //////////////////////////////////////////////

            // Render maps at centered positions
            for (int i = 0; i < map_occurence; i++)
            {
                int mapX = x_start + i * (mapWidth + 40);
                int mapY = (gScreenSurface->h - maps[i]->h) / 2;

                int x, y;
                SDL_GetMouseState(&x, &y);

                // Check if the mouse is inside the map area
                if (isMouseInside(x, y, mapX, maps[i]))
                {
                    // Render hover map
                    int hoverMapY = gScreenSurface->h - (gScreenSurface->h - mapY) - maps[i]->h / 8;
                    int hoverMapX = mapX + (mapWidth - maps[i]->w) / 2;
                    renderMap(maps[i], hoverMapX, hoverMapY);
                }
                else
                {
                    // Render normal map
                    renderMap(maps[i], mapX, mapY);
                }

                // Render text below the map (centered)
                int textWidth, textHeight;
                TTF_SizeText(font, map_chosen[i], &textWidth, &textHeight);

                // Add space before each uppercase letter in map_chosen
                char title_chosen[map_occurence][20];
                strcpy(title_chosen[i], map_chosen[i]);
                addSpaceBeforeUppercase(title_chosen[i]);

                int textX = mapX + (mapWidth - textWidth) / 2;
                int textY = mapY + maps[i]->h + 20;

                renderText(gScreenSurface, title_chosen[i], textX, textY);
            }

            // Update the surface
            SDL_UpdateWindowSurface(gWindow);
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}
