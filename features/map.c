#include "../headers/include.h"
#include "../headers/map.h"

char *map(int day, char *background_image)
{
    //////////////////////////////////////////////
    // Load background
    SDL_Surface *background = loadMedia(background_image, 1);
    //////////////////////////////////////////////
    srand(time(NULL));
    char map_choices[5][20] = {
        "Common",
        "Elite",
        "Treasure",
        "Camp",
        "Boss",
    };
    char countries[5][20] = {"France", "Germany", "China", "Korea", "Japan"};
    int map_occurrence = rand() % 2 + 2;
    char map_chosen[3][20];
    SDL_Surface *maps[map_occurrence];

    //////////////////////////////////////////////
    // printf("map_occurrence: %d\n", map_occurrence);
    //////////////////////////////////////////////

    map_occurrence = generateMaps(day, map_choices, countries, map_occurrence, map_chosen, maps);

    //////////////////////////////////////////////
    for (int i = 0; i < map_occurrence; i++)
    {
        // printf("Number %d : %s\n", i, map_chosen[i]);
    }
    //////////////////////////////////////////////

    int screenWidth = gScreenSurface->w;
    int totalMapsWidth = 0;

    // Assuming all maps have the same width
    int mapWidth = maps[0]->w;
    int spacing = 40; // 40 pixels

    for (int i = 0; i < map_occurrence; i++)
    {
        totalMapsWidth += mapWidth + spacing;
    }

    int x_start = (screenWidth - totalMapsWidth + spacing) / 2;

    // Check if action has been performed
    bool end_loop = false;
    // Event handler
    SDL_Event e;

    while (!end_loop)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                printf("\n\nQuiting ...\n\n");
                close();
            }

            if (e.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (int i = 0; i < map_occurrence; i++)
                {
                    maps[i] = isMouseInside(x, y, x_start + i * (mapWidth + 40), maps[i]) ? loadMedia(map_chosen[i], 6) : loadMedia(map_chosen[i], 5);
                    // isMouseInside(x, y, x_start + i * (mapWidth + 40), maps[i]) ? printf("%s\n", map_chosen[i]) : 0;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (SDL_BUTTON_LEFT == e.button.button)
                {
                    //////////////////////////////////////////////
                    // printf("Left mouse button is down\n");
                    //////////////////////////////////////////////

                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    for (int i = 0; i < map_occurrence; i++)
                    {
                        if (isMouseInside(x, y, x_start + i * (mapWidth + 40), maps[i]))
                        {
                            char *returned_map = malloc(strlen(map_chosen[i]) + 1);
                            strcpy(returned_map, map_chosen[i]);
                            return returned_map;
                            end_loop = true;
                        }
                    }
                }
            }
        }

        // Clear the screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

        //////////////////////////////////////////////
        // Render background
        renderMap(background, 0, 0, 0, 0);
        //////////////////////////////////////////////

        char dayText[50];
        sprintf(dayText, "Day %d", day);
        renderText(gScreenSurface, dayText, 64, 2, 4.5);
        renderText(gScreenSurface, "Please choose your next destination", 32, 2, 1.15);

        // Render maps at centered positions
        for (int i = 0; i < map_occurrence; i++)
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
                renderMap(maps[i], hoverMapX, hoverMapY, 0, 0);
            }
            else
            {
                // Render normal map
                renderMap(maps[i], mapX, mapY, 0, 0);
            }

            // Render text below the map (centered)
            int textWidth, textHeight;
            TTF_SizeText(font, map_chosen[i], &textWidth, &textHeight);

            // Add space before each uppercase letter in map_chosen
            char title_chosen[map_occurrence][20];
            strcpy(title_chosen[i], map_chosen[i]);
            addSpaceBeforeUppercase(title_chosen[i]);

            int textX = mapX + (mapWidth - textWidth) / 2;
            int textY = mapY + maps[i]->h + 20;

            renderTextChosen(gScreenSurface, title_chosen[i], textX, textY);
        }

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }
}

SDL_Surface *loadMedia(char *path, int scale)
{
    char full_path[255] = "data/maps/";
    strcat(full_path, path);
    char ext[5] = ".png";
    strcat(full_path, ext);

    // Load splash image
    SDL_Surface *originalSurface = IMG_Load(full_path);
    if (originalSurface == NULL)
    {
        printf("%s\n", path);
        printf("Unable to load image %s! SDL Error: %s\n", full_path, SDL_GetError());
        close();
    }
    // Set up alpha blending
    // SDL_SetSurfaceBlendMode(originalSurface, SDL_BLENDMODE_BLEND);

    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == NULL)
    {
        printf("Unable to convert surface! SDL Error: %s\n", SDL_GetError());
        close();
    }

    // Calculate the desired width and height based on the scale factor
    int desiredWidth = formattedSurface->w * scale;
    int desiredHeight = formattedSurface->h * scale;

    // Create a new surface with the desired size
    SDL_Surface *scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, desiredWidth, desiredHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (scaledSurface == NULL)
    {
        printf("Unable to scale surface! SDL Error: %s\n", SDL_GetError());
        close();
    }

    SDL_BlitScaled(formattedSurface, NULL, scaledSurface, NULL);

    // Free the original surface as it is no longer needed
    SDL_FreeSurface(originalSurface);
    SDL_FreeSurface(formattedSurface);

    return scaledSurface;
}

int generateMaps(int day, char map_choices[][20], char countries[][20], int map_occurrence, char map_chosen[][20], SDL_Surface *maps[])
{
    for (int i = 0; i < map_occurrence; i++)
    {
        int randomNumber = rand() % 10;

        if ((day) % 10 == 0)
        {
            randomNumber = 10;
            map_occurrence = 1;
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

        // check if already in map_chosen
        for (int j = 0; j < i; j++)
        {
            if (strcmp(map_chosen[i], map_chosen[j]) == 0)
            {
                i--;
            }
        }

        maps[i] = loadMedia(map_chosen[i], 5);
    }
    return map_occurrence;
}

void renderMap(SDL_Surface *map, int x, int y, int w, int h)
{
    SDL_Rect destRect = {x, y, w, h};
    SDL_BlitSurface(map, NULL, gScreenSurface, &destRect);
}

void renderText(SDL_Surface *surface, char *Text, int font_size, float width_divide, float height_divide)
{
    // Increase the font size and set the text color
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    TTF_SetFontSize(font, font_size); // Set your desired font size

    SDL_Color textColor = {255, 255, 255}; // White color

    // Render the day text
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, Text, textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    int textX = (surface->w - textSurface->w) / width_divide;
    int textY = (surface->h - textSurface->h) / height_divide; // Adjust the vertical position

    renderMap(textSurface, textX, textY, textSurface->w, textSurface->h);

    SDL_FreeSurface(textSurface);

    // Restore the original font size
    TTF_SetFontSize(font, 24); // Set the original font size
}

bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map)
{
    int screen_height = gScreenSurface->h;
    int map_height = map->h;

    return mouseX >= x_start && mouseX <= x_start + map->w && mouseY >= (screen_height - map_height) / 2 && mouseY <= (screen_height - map_height) / 2 + map_height;
}

void renderTextChosen(SDL_Surface *surface, const char *text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255}; // White color

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    renderMap(textSurface, x, y, textSurface->w, textSurface->h);

    SDL_FreeSurface(textSurface);
}

void addSpaceBeforeUppercase(char *str)
{
    char result[255];
    int resultIndex = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isupper(str[i]))
        {
            if (i > 0 && !isspace(str[i - 1]))
            {
                result[resultIndex++] = ' ';
            }
        }
        result[resultIndex++] = str[i];
    }

    result[resultIndex] = '\0';

    strcpy(str, result);
}
