#include "headers/include.h"

// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// The Font
TTF_Font *font = NULL;

#include "features/default.c"
#include "features/map.c"
#include "features/combat.c"

int main(int argc, char *args[])
{
    init();

    while (1)
    {
        char *selected_map = map();
        printf("Selected: %s\n\n", selected_map);
        if (strcmp(selected_map, "Camp") == 0 || strcmp(selected_map, "Treasure") == 0)
        {
            printf("You have selected camp or treasure.\n\n");
            close();
        }
        else
        {
            combat(selected_map);
        }
    }

    close();
    return 0;
}