#include "headers/include.h"

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// The Font
TTF_Font *font = NULL;
bool quit = false;

#include "features/default.c"
#include "features/map.c"

int main(int argc, char *args[])
{
    init();

    // While application is running
    while (!quit)
    {
        int day = 0;

        for (; day < 100; day++)
        {
            map(day);
        }
    }

    close();
}