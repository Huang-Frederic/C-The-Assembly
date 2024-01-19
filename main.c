#include "headers/include.h"

#include "features/default.c"
#include "features/map.c"
#include "features/combat.c"
#include "features/event.c"

int main(int argc, char *args[])
{
    init();

    while (1)
    {
        char *selected_map = map();
        // printf("Selected: %s\n\n", selected_map);
        event(selected_map);
    }

    close_SDL();
    return 0;
}