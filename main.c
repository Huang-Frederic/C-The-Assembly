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
    // insert(1500, "Gautier");
    // insert(2700, "Hisshiden");
    // insert(3000, "Stephane");
    // insert(4200, "Franck");
    // insert(101001, "Mr-Neveu");
    while (1)
    {
        display_menu();
        // event("Treasure");
    }
}
