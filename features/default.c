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
            {
                printf("\nError Renderer : %s", SDL_GetError());
                printf("\nThere was a problem creating the renderer.");
                close_SDL();
            }
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            SDL_Surface *icon = IMG_Load("data/Logo.png");
            SDL_SetWindowIcon(gWindow, icon);
        }
    }

    get_auto_save();
    get_display_errors();
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

    if (auto_save_on == 0)
    {
        FILE *fp = fopen("data/player_save.txt", "r");
        FILE *ft = fopen("data/save.txt", "w");
        if (ft != NULL && fp != NULL)
        {
            int ch;
            while ((ch = fgetc(fp)) != EOF)
            {
                fputc(ch, ft);
            }

            fclose(ft);
            fclose(fp);
        }
    }
    remove("data/player_save.txt");

    // Quit SDL subsystems
    SDL_Quit();
}

void get_auto_save()
{
    // use fscanf to read save.txt file
    char line[255];
    int Found = 0;

    FILE *ini_file = fopen("c.ini", "r");
    if (ini_file == NULL)
    {
        printf("An error while opening c.ini have occured!\n");
        close_SDL();
    }

    while (fscanf(ini_file, " %[^\n]", line) != EOF)
    {
        if (strstr(line, "auto_save = On") != NULL)
        {
            auto_save_on = 1;
            Found = 1;
        }
        else if (strstr(line, "auto_save = Off") != NULL)
        {
            auto_save_on = 0;
            Found = 1;
        }
    }

    if (!Found)
    {
        printf("auto_save in c.ini not found, auto_save have been set to On\n\n");
    }

    fclose(ini_file);
}

void get_display_errors()
{
    // use fscanf to read save.txt file
    char line[255];
    int Found = 0;

    FILE *ini_file = fopen("c.ini", "r");
    if (ini_file == NULL)
    {
        printf("An error while opening c.ini have occured!\n");
        close_SDL();
    }

    while (fscanf(ini_file, " %[^\n]", line) != EOF)
    {
        if (strstr(line, "display_errors = On") != NULL)
        {
            display_errors_on = 1;
            Found = 1;
        }
        else if (strstr(line, "display_errors = Off") != NULL)
        {
            display_errors_on = 0;
            Found = 1;
        }
    }

    if (!Found)
    {
        printf("display_errors in c.ini not found, display_errors have been set to On\n\n");
    }

    fclose(ini_file);
}

SDL_Surface *load_Pathed_Media(char *path, float scale)
{
    char full_path[255] = "data/";
    strcat(full_path, path);
    char ext[5] = ".png";
    strcat(full_path, ext);

    // Load splash image
    SDL_Surface *originalSurface = IMG_Load(full_path);
    if (originalSurface == NULL)
    {
        printf("%s\n", path);
        printf("Unable to load image %s! SDL Error: %s\n", full_path, SDL_GetError());
        close_SDL();
    }

    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == NULL)
    {
        printf("Unable to convert surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    int desiredWidth = formattedSurface->w * scale;
    int desiredHeight = formattedSurface->h * scale;

    SDL_Surface *scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, desiredWidth, desiredHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (scaledSurface == NULL)
    {
        printf("Unable to scale surface! SDL Error: %s\n", SDL_GetError());
        close_SDL();
    }

    SDL_BlitScaled(formattedSurface, NULL, scaledSurface, NULL);

    SDL_FreeSurface(originalSurface);
    SDL_FreeSurface(formattedSurface);

    return scaledSurface;
}

bool check_if_wanna_save()
{
    SDL_Event e;
    int clicked = 0;
    SDL_Surface *screenCopy = SDL_ConvertSurface(gScreenSurface, gScreenSurface->format, 0);
    SDL_SetSurfaceAlphaMod(screenCopy, 255);
    SDL_BlitSurface(screenCopy, NULL, gScreenSurface, NULL);

    SDL_Surface *yes = load_Pathed_Media("others/yes", 1);
    SDL_Surface *no = load_Pathed_Media("others/no", 1);

    SDL_Rect Rect_yes = {350, gScreenSurface->h / 2, 0, 0};
    SDL_BlitSurface(yes, NULL, gScreenSurface, &Rect_yes);
    SDL_Rect Rect_no = {750, gScreenSurface->h / 2, 0, 0};
    SDL_BlitSurface(no, NULL, gScreenSurface, &Rect_no);

    loadFont(selectedFont, 48);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Solid(font, "Do you wan't to save ?", textColor);
    if (message != NULL)
    {
        SDL_Rect textRect = {370, gScreenSurface->h / 2 - 100, 0, 0};
        SDL_BlitSurface(message, NULL, gScreenSurface, &textRect);
        SDL_FreeSurface(message);
    }

    SDL_UpdateWindowSurface(gWindow);

    // FadeEffect(0, 0);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {

            if (e.type == SDL_QUIT)
            {
                printf("\n\nQuiting ...\n\n");
                close_SDL();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // yes
                int y = gScreenSurface->h / 2;
                if (mouseX >= Rect_yes.x && mouseX < Rect_yes.x + Rect_yes.w && mouseY >= Rect_yes.y && mouseY < Rect_yes.y + Rect_yes.h)
                {
                    return true;
                    break;
                }
                // no
                else if (mouseX >= Rect_no.x && mouseX < Rect_no.x + Rect_no.w && mouseY >= Rect_no.y && mouseY < Rect_no.y + Rect_no.h)
                {
                    return false;
                    break;
                }

                clicked = 1;
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                // yes
                int y = gScreenSurface->h / 2;
                if (mouseX >= Rect_yes.x && mouseX < Rect_yes.x + Rect_yes.w && mouseY >= Rect_yes.y && mouseY < Rect_yes.y + Rect_yes.h)
                {
                    yes = load_Pathed_Media("others/yes-hover", 1);
                    no = load_Pathed_Media("others/no", 1);
                }
                else if (mouseX >= Rect_no.x && mouseX < Rect_no.x + Rect_no.w && mouseY >= Rect_no.y && mouseY < Rect_no.y + Rect_no.h)
                {
                    no = load_Pathed_Media("others/no-hover", 1);
                    yes = load_Pathed_Media("others/yes", 1);
                }
                else
                {
                    yes = load_Pathed_Media("others/yes", 1);
                    no = load_Pathed_Media("others/no", 1);
                }
            }

            SDL_BlitSurface(yes, NULL, gScreenSurface, &Rect_yes);
            SDL_BlitSurface(no, NULL, gScreenSurface, &Rect_no);
        }
        SDL_UpdateWindowSurface(gWindow);
    }
}

void save_to_player()
{
    FILE *fp = fopen("data/save.txt", "r");
    FILE *ft = fopen("data/player_save.txt", "w");
    if (ft != NULL && fp != NULL)
    {
        int ch;
        while ((ch = fgetc(fp)) != EOF)
        {
            fputc(ch, ft);
        }

        fclose(fp);
        fclose(ft);
    }
}