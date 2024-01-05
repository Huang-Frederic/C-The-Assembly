// Using SDL and standard IO
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
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
TTF_Font *font = NULL;

// Init Functions
void init();
SDL_Surface *loadMedia(char *path, int scale);
void renderMap(SDL_Surface *map, int x, int y);
bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map);
bool loadFont();
void renderText(SDL_Surface *surface, const char *text, int x, int y);
void addSpaceBeforeUppercase(char *str);
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
    else if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        close();
    }
    else if (!loadFont())
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        close();
    }
    else
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        // Create window
        gWindow = SDL_CreateWindow("Slay The Assembly", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
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

bool loadFont()
{
    font = TTF_OpenFont("ARCADECLASSIC.TTF", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void renderText(SDL_Surface *surface, const char *text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255}; // White color

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
    SDL_BlitSurface(textSurface, NULL, surface, &destRect);

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

void close()
{
    // Deallocate surface
    // LOOP FOR FREING ALL SURFACES
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}
