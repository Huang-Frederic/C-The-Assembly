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

    int day = 1;
    // for (; day < 100; day++)
    // {
    //     printf("Day: %d\n", day);
    //     char *selected_map = map(day, "background2");
    //     printf("Selected: %s\n\n", selected_map);
    // }
    char *selected_map = "Elite_EX";
    combat(selected_map);

    close();
    return 0;
}