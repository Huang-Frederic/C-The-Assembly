#include "headers/include.h"

#include "features/default.c"
#include "features/database.c"
#include "features/weather.c"
#include "features/map.c"
#include "features/combat.c"
#include "features/event.c"

#include "features/menu.c"
#include "features/input.c"

int main(int argc, char *args[])
{
    init();

    // delete_all();
    // insert(100, "Gautier");
    // insert(200, "Hisshiden");
    // insert(300, "Stephane");
    // insert(400, "Franck");
    // insert(500, "Donnesh");
    while (1)
    {
        display_menu();
        // event("Camp");
    }

    close_SDL();
    return 0;
}
