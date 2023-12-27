// Using SDL and standard IO
#include <SDL2\SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2\SDL_image.h>

// Init Functions
bool init();
bool loadMedia();
void close();

// Key press surfaces constants Default = 0, Up = 1, Down = 2, Left = 3, Right = 4, Total = 5
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Screen dimension constants
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// Loads individual image
SDL_Surface *loadSurface(const char *path);
// The images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
// Current displayed image
SDL_Surface *gCurrentSurface = NULL;
// Current displayed image
SDL_Surface *gStretchedSurface = NULL;

int main(int argc, char *args[])
{
    SDL_Surface *image = IMG_Load("image.png");
    if (!image)
    {
        printf("Erreur de chargement de l'image : %s", SDL_GetError());
        return -1;
    }

    // Initialize SDL and load media
    if (!init() || !loadMedia())
    {
        printf("Failed to initialize or load media!\n");
        return 1; // Return an error code
    }

    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;
    // Set default current surface
    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

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
            // User presses a key
            else if (e.type == SDL_KEYDOWN)
            {
                // Select surfaces based on key press
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                    break;

                case SDLK_DOWN:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                    break;

                case SDLK_LEFT:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                    break;

                case SDLK_RIGHT:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                    break;

                default:
                    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                    break;
                }
            }
        }

        // Apply the image stretched
        SDL_Rect stretchRect;
        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = SCREEN_WIDTH;
        stretchRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }
    close();
    return 0;
}

bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
            else
            {
                // Get window surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }

    return success;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("loaded.png");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    // //Load up surface
    // gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "maps/Rathalos.bmp" );
    // if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    // {
    //     printf( "Failed to load up image!\n" );
    //     success = false;
    // }

    // //Load down surface
    // gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "maps/Lagiacrus.bmp" );
    // if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    // {
    //     printf( "Failed to load down image!\n" );
    //     success = false;
    // }

    // //Load left surface
    // gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "maps/Kirin.bmp" );
    // if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    // {
    //     printf( "Failed to load left image!\n" );
    //     success = false;
    // }

    // //Load right surface
    // gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "maps/Teostra.bmp" );
    // if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    // {
    //     printf( "Failed to load right image!\n" );
    //     success = false;
    // }

    return success;
}

void close()
{
    // Deallocate surface
    SDL_FreeSurface(gStretchedSurface);
    gStretchedSurface = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

SDL_Surface *loadSurface(const char *path)
{
    // The final optimized image
    SDL_Surface *optimizedSurface = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    }
    else
    {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}