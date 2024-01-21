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
    // struct Save save;
    // init_save(&save);
    // create_save(save);
    init();

    // delete_all();
    // insert(100, "Gautier");
    // insert(200, "Hisshiden");
    // insert(300, "Stephane");
    // insert(400, "Franck");
    // insert(500, "Donnesh");

    display_menu();

    // while (1)
    // {
    //     char *selected_map = map();
    //     printf("Selected: %s\n\n", selected_map);
    //     event(selected_map);
    // }

    // close_SDL();
    return 0;
}
