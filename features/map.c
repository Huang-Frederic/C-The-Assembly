#include "../headers/include.h"
#include "../headers/map.h"

char *map()
{
    char *background_image = "Map";
    int day = get_save_day();

    srand(time(NULL));
    char map_choices[5][20] = {
        "Common",
        "Elite",
        "Treasure",
        "Camp",
        "Boss",
    };
    char countries[5][20] = {"France", "USA", "China", "Korea", "Japan"};
    int map_occurrence = rand() % 2 + 2;
    char map_chosen[3][20];
    SDL_Surface *background = load_Background_Media_Map(background_image);
    SDL_Surface *maps[map_occurrence];
    int faded = 0;

    map_occurrence = generateMaps(day, map_choices, countries, map_occurrence, map_chosen, maps);

    int screenWidth = gScreenSurface->w;
    int totalMapsWidth = 0;

    // Assuming all maps have the same width
    int mapWidth = maps[0]->w;
    int spacing = 200;

    for (int i = 0; i < map_occurrence; i++)
    {
        totalMapsWidth += mapWidth + spacing;
    }

    int x_start = (screenWidth - totalMapsWidth) / 2 + spacing / 2;

    bool end_loop = false;
    SDL_Event e;

    while (!end_loop)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                if (display_errors_on)
                    fprintf(stderr, "\n\nQuiting ...\n\n");
                close_SDL();
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (SDL_BUTTON_LEFT == e.button.button)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    for (int i = 0; i < map_occurrence; i++)
                    {
                        if (isMouseInside(x, y, x_start + i * (spacing + maps[i]->w), maps[i]))
                        {
                            char *returned_map = malloc(strlen(map_chosen[i]) + 1);
                            strcpy(returned_map, map_chosen[i]);

                            FadeEffect(faded, 1);
                            SDL_Delay(100);
                            end_loop = true;
                            return returned_map;
                        }
                    }
                }
            }
        }

        // Clear the screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

        // Render background
        renderMap(background, 0, 0, 0, 0);

        char dayText[50];
        sprintf(dayText, "Day %d", day);
        renderMapText(gScreenSurface, dayText, 64, 2, 4.5);
        renderMapText(gScreenSurface, "Please choose your next destination", 32, 2, 1.15);
        display_map_score();

        // Render maps at centered positions
        for (int i = 0; i < map_occurrence; i++)
        {
            int mapX = x_start + i * (spacing + maps[i]->w);
            int mapY = (gScreenSurface->h - maps[i]->h) / 2;

            int x, y;
            SDL_GetMouseState(&x, &y);

            // Render text below the map (centered)
            int textWidth, textHeight;
            TTF_SizeText(font, map_chosen[i], &textWidth, &textHeight);

            // Add space before each uppercase letter in map_chosen
            char title_chosen[map_occurrence][20];
            strcpy(title_chosen[i], map_chosen[i]);
            addSpaceBeforeUppercase(title_chosen[i]);

            int textX = mapX + (maps[i]->w - textWidth) / 2;
            int textY = mapY + maps[i]->h + 20;

            // Check if the mouse is inside the map area
            if (isMouseInside(x, y, mapX, maps[i]))
            {
                // Render hover map
                renderMap(maps[i], mapX + 10, mapY - 10, 0, 0);
                renderTextChosen(gScreenSurface, title_chosen[i], textX, textY + 10);
            }
            else
            {
                // Render normal map
                renderMap(maps[i], mapX + 10, mapY, 0, 0);
                renderTextChosen(gScreenSurface, title_chosen[i], textX, textY);
            }
        }

        faded = FadeEffect(faded, 0);
        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }
}

SDL_Surface *load_Background_Media_Map(char *background_image)
{
    char background_path[256] = "backgrounds/";
    strcat(background_path, background_image);
    return load_Pathed_Media_Map(background_path, 1);
}

SDL_Surface *load_Pathed_Media_Map(char *path, float scale)
{
    char full_path[255] = "data/";
    strcat(full_path, path);
    char ext[5] = ".png";
    strcat(full_path, ext);

    // Load splash image
    SDL_Surface *originalSurface = IMG_Load(full_path);
    if (originalSurface == NULL)
    {
        if (display_errors_on) {
            printf("%s\n", path);
            printf("Unable to load image %s! SDL Error: %s\n", full_path, SDL_GetError());
        }
        close_SDL();
    }

    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Unable to convert surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    int desiredWidth = formattedSurface->w * scale;
    int desiredHeight = formattedSurface->h * scale;

    SDL_Surface *scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, desiredWidth, desiredHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (scaledSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Unable to scale surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    SDL_BlitScaled(formattedSurface, NULL, scaledSurface, NULL);

    SDL_FreeSurface(originalSurface);
    SDL_FreeSurface(formattedSurface);

    return scaledSurface;
}

SDL_Surface *loadMedia(char *path, float scale)
{
    char full_path[255] = "data/maps/";
    strcat(full_path, path);
    char ext[5] = ".png";
    strcat(full_path, ext);

    // Load splash image
    SDL_Surface *originalSurface = IMG_Load(full_path);
    if (originalSurface == NULL)
    {
        if (display_errors_on) {
            fprintf(stderr, "%s\n", path);
            fprintf(stderr, "Unable to load image %s! SDL Error: %s\n", full_path, SDL_GetError());
        }
        close_SDL();
    }

    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Unable to convert surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    int desiredWidth = formattedSurface->w * scale;
    int desiredHeight = formattedSurface->h * scale;

    SDL_Surface *scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, desiredWidth, desiredHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (scaledSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Unable to scale surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    SDL_BlitScaled(formattedSurface, NULL, scaledSurface, NULL);

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

        maps[i] = loadMedia(map_chosen[i], 0.8);
    }
    return map_occurrence;
}

void renderMap(SDL_Surface *map, int x, int y, int w, int h)
{
    SDL_Rect destRect = {x, y, w, h};
    SDL_BlitSurface(map, NULL, gScreenSurface, &destRect);
}

void renderMapText(SDL_Surface *surface, char *Text, int font_size, float width_divide, float height_divide)
{
    // Increase the font size and set the text color
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
    TTF_SetFontSize(font, font_size); // Set your desired font size

    SDL_Color textColor = {255, 255, 255}; // White color

    // Render the day text
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, Text, textColor);
    if (textSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
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
        if (display_errors_on)
            fprintf(stderr, "Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
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
            result[resultIndex] = ' ';
            resultIndex++;
        }
        result[resultIndex] = str[i];
        resultIndex++;
    }

    result[resultIndex] = '\0';

    strcpy(str, result);
}

int FadeEffect(int faded, int inOut)
{
    if (inOut == 0)
    {
        if (faded == 0)
        {
            SDL_Surface *screenCopy = SDL_ConvertSurface(gScreenSurface, gScreenSurface->format, 0);
            SDL_Surface *overlay = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
            int reduce = 4;
            for (int alpha = 255; alpha > 0; alpha -= reduce)
            {
                // make a copy of gScreenSurface
                SDL_SetSurfaceAlphaMod(screenCopy, 255);
                SDL_BlitSurface(screenCopy, NULL, gScreenSurface, NULL);

                // Render the semi-transparent black overlay on top
                SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, alpha));
                SDL_BlitSurface(overlay, NULL, gScreenSurface, NULL);

                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);

                reduce = alpha > 150 ? 4 : 8;
            }
            SDL_FreeSurface(screenCopy);

            return 1;
        }
        else
        {
            return 1;
        }
    }
    else if (inOut == 1)
    {
        SDL_Surface *screenCopy = SDL_ConvertSurface(gScreenSurface, gScreenSurface->format, 0);
        SDL_Surface *overlay = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
        int reduce = 5;
        for (int alpha = 0; alpha < 255; alpha += reduce)
        {
            // make a copy of gScreenSurface
            SDL_SetSurfaceAlphaMod(screenCopy, 255);
            SDL_BlitSurface(screenCopy, NULL, gScreenSurface, NULL);

            // Render the semi-transparent black overlay on top
            SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 0, 0, 0, alpha));
            SDL_BlitSurface(overlay, NULL, gScreenSurface, NULL);

            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(20);

            reduce = alpha < 150 ? 5 : 10;
        }
        SDL_FreeSurface(screenCopy);
        SDL_FreeSurface(overlay);

        return 0;
    }
}

int get_save_day()
{
    FILE *save = fopen("data/save.txt", "r");
    if (save == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Error opening file save.txt for getting the day!\n");
        close_SDL();
    }
    int day;
    for (int i = 0; i < 2; i++)
    {
        fscanf(save, "%d", &day);
    }
    fclose(save);
    if (display_errors_on)
        fprintf(stderr, "Day: %d\n", day);
    return day;
}

void display_map_score()
{

    int player_score = 0;

    FILE *save_file = fopen("data/save.txt", "r");
    if (save_file == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Error opening file the save file during map!\n");
        close_SDL();
    }
    else
    {
        fscanf(save_file, "%*d %*d %*s %*d %*d %*d %*d %d", &player_score);
        fclose(save_file);
    }

    // renderCombatText("Click to open the chest", 430, gScreenSurface->h - 200, 32);
    // void renderCombatText(const char *text, int x, int y, int font_size)
    char text[100];
    sprintf(text, "Score : %d", player_score);

    loadFont(selectedFont, 32);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Solid(font, text, textColor);
    int x = gScreenSurface->w - message->w - 30;
    int y = 20;

    if (message != NULL)
    {
        SDL_Rect textRect = {x, y, 0, 0};
        SDL_BlitSurface(message, NULL, gScreenSurface, &textRect);
        SDL_FreeSurface(message);
    }
}