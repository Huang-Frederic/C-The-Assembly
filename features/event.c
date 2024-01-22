#include "../headers/include.h"
#include "../headers/event.h"

void event(char *map)
{
    if (strcmp(map, "Camp") == 0)
    {
        if (display_errors_on)
            fprintf(stderr, "You have entered a camp!\n");

        display_camp();
        save_camp();
    }
    else if (strcmp(map, "Treasure") == 0)
    {
        srand(time(NULL));
        if (display_errors_on)
            fprintf(stderr, "You have found a treasure!\n");

        save_treasure(get_treasure());
    }
    else
    {
        if (display_errors_on)
            fprintf(stderr, "You have entered a combat!\n");
        combat(map);
    }
}

void display_camp()
{
    // Display the camp screen
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
    renderCombatText("You have entered a camp", 420, gScreenSurface->h / 2 - 90, 32);
    renderCombatText("Your have been healed for 75% of your maximum HP", 210, gScreenSurface->h / 2 - 60, 32);
    renderMap(load_Pathed_Media("maps/Camp", 1), 550, gScreenSurface->h / 2, 0, 0);
    SDL_UpdateWindowSurface(gWindow);
    FadeEffect(0, 0);
    SDL_Delay(1000);
    FadeEffect(0, 1);
}

void save_camp()
{
    struct Save save;

    ////////////////////////////////
    int wanna_save = 1;
    if (auto_save_on == 0)
        if (!check_if_wanna_save())
            wanna_save = 0;
    ////////////////////////////////

    if (wanna_save)
    {
        // Display the beginning of the save screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
        renderCombatText("Saving .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        renderMap(load_Pathed_Media("Logo", 0.20), gScreenSurface->w - 255, gScreenSurface->h - 65, 0, 0);
        FadeEffect(0, 0);
        renderCombatText("Saving . .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        SDL_UpdateWindowSurface(gWindow);
    }

    // Get the save data
    FILE *save_file = fopen("data/save.txt", "r");
    if (save_file == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "Error opening file the save file during camp save!\n");
        close_SDL();
    }
    else
    {
        fscanf(save_file, "%d %d %s %d %d %d %d %d", &save.deck_size, &save.day, save.player_name, &save.difficulty, &save.hp, &save.max_hp, &save.max_energy, &save.score);
        char deck[save.deck_size][30];
        for (int i = 0; i < save.deck_size; i++)
        {
            fscanf(save_file, "%s", deck[i]);
        }
        fclose(save_file);

        // Apply the changes
        save.hp = save.hp + (save.max_hp * 0.75) > save.max_hp ? save.max_hp : save.hp + (save.max_hp * 0.75);
        save.score += 20 + (20 * (save.day * 0.05)) + (20 * (save.difficulty == -1 ? -0.25 : save.difficulty == 1 ? 0.25
                                                                                                                  : 0));
        save.day++;

        // write to file
        save_file = fopen("data/save.txt", "w");
        if (save_file == NULL)
        {
            if (display_errors_on)
                fprintf(stderr, "Error opening file the save file during camp save!\n");
            close_SDL();
        }
        else
        {
            fprintf(save_file, "%d\n%d\n%s\n%d\n%d\n%d\n%d\n%d\n", save.deck_size, save.day, save.player_name, save.difficulty, save.hp, save.max_hp, save.max_energy, save.score);
            for (int i = 0; i < save.deck_size; i++)
            {
                fprintf(save_file, "%s\n", deck[i]);
            }
            fclose(save_file);
        }
    }

    if (wanna_save)
    {
        save_to_player();
        // Display the end of the save screen
        SDL_Delay(500);
        renderCombatText("Saving . . .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        SDL_UpdateWindowSurface(gWindow);
        FadeEffect(0, 1);
    }
}

struct Treasure get_treasure()
{
    struct Treasure treasure;
    srand((unsigned int)time(NULL));

    // check there is > 28 energy in the save file
    FILE *save_file = fopen("data/save.txt", "r");
    if (save_file == NULL)
    {
        if (display_errors_on)
        fprintf(stderr, "Error opening file the save file during treasure!\n");
        close_SDL();
    }
    else
    {
        int max_energy = 0;
        treasure.energy = 0;
        fscanf(save_file, "%*d %*d %*s %*d %*d %*d %d", &max_energy);
        fclose(save_file);

        if (max_energy == 28)
        {
            if (display_errors_on)
            fprintf(stderr, "Max Energy Amount reached !\n");
            treasure.card = strdup(get_treasure_card());
            treasure.if_no_energy_card = get_treasure_card();
        }
        else
        {
            treasure.energy += 1;
            treasure.card = get_treasure_card();
            treasure.if_no_energy_card = NULL;
        }
    }

    return treasure;
}

char *get_treasure_card()
{
    // Check how many .txt in cards folder
    int number_of_cards = 0;

    DIR *d;
    struct dirent *dir;
    d = opendir("data/cards/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".txt") != NULL && strncmp(dir->d_name, "__Player__", strlen("__Player__")) == 0)
            {
                number_of_cards++;
            }
        }
        closedir(d);
    }

    // Get a random number between 0 and number_of_cards
    int random_number = rand() % number_of_cards;
    number_of_cards = 0;

    d = opendir("data/cards/");
    if (d != NULL)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".txt") != NULL && strncmp(dir->d_name, "__Player__", strlen("__Player__")) == 0)
            {
                if (random_number == number_of_cards++)
                {
                    closedir(d);
                    char *dot = strchr(dir->d_name, '.');
                    if (dot != NULL)
                    {
                        *dot = '\0';
                    }
                    return dir->d_name;
                }
            }
        }
    }
}

void save_treasure(struct Treasure treasure)
{
    display_treasure(treasure);

    struct Save save;

    ////////////////////////////////
    int wanna_save = 1;
    if (auto_save_on == 0)
        if (!check_if_wanna_save())
            wanna_save = 0;
    ////////////////////////////////

    if (wanna_save)
    {
        // Display the beginning of the save screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
        renderCombatText("Saving .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        renderMap(load_Pathed_Media("Logo", 0.20), gScreenSurface->w - 255, gScreenSurface->h - 65, 0, 0);
        FadeEffect(0, 0);
        renderCombatText("Saving . .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        SDL_UpdateWindowSurface(gWindow);
    }

    // Get the save data
    FILE *save_file = fopen("data/save.txt", "r");
    if (save_file == NULL)
    {
        if (display_errors_on)
        fprintf(stderr, "Error opening file the save file during camp save!\n");
        close_SDL();
    }
    else
    {
        fscanf(save_file, "%d %d %s %d %d %d %d %d", &save.deck_size, &save.day, save.player_name, &save.difficulty, &save.hp, &save.max_hp, &save.max_energy, &save.score);
        char deck[save.deck_size][30];
        for (int i = 0; i < save.deck_size; i++)
        {
            fscanf(save_file, "%s", deck[i]);
        }
        fclose(save_file);

        // Apply the changes
        save.max_energy = save.max_energy + treasure.energy > 28 ? 28 : save.max_energy + treasure.energy;
        save.score += 10 + (10 * (save.day * 0.05)) + (10 * (save.difficulty == -1 ? -0.25 : save.difficulty == 1 ? 0.25
                                                                                                                  : 0));
        save.day++;
        int new_cards = treasure.if_no_energy_card == NULL ? 1 : 2;
        save.deck_size = save.deck_size + new_cards;

        // write to file
        save_file = fopen("data/save.txt", "w");
        if (save_file == NULL)
        {
            if (display_errors_on)
            fprintf(stderr, "Error opening file the save file during treasure save!\n");
            close_SDL();
        }
        else
        {
            fprintf(save_file, "%d\n%d\n%s\n%d\n%d\n%d\n%d\n%d\n", save.deck_size, save.day, save.player_name, save.difficulty, save.hp, save.max_hp, save.max_energy, save.score);
            for (int i = 0; i < save.deck_size - new_cards; i++)
            {
                fprintf(save_file, "%s\n", deck[i]);
            }
            fprintf(save_file, "%s\n", treasure.card);
            if (treasure.if_no_energy_card != NULL)
            {
                fprintf(save_file, "%s\n", treasure.if_no_energy_card);
            }

            fclose(save_file);
        }
    }

    if (wanna_save)
    {
        save_to_player();
        // Display the end of the save screen
        SDL_Delay(500);
        renderCombatText("Saving . . .", gScreenSurface->w - 200, gScreenSurface->h - 60, 32);
        SDL_UpdateWindowSurface(gWindow);
        FadeEffect(0, 1);
    }
}

void display_treasure(struct Treasure treasure)
{
    if (display_errors_on) {
        fprintf(stderr,  "T1 : %s\n", treasure.card);
        fprintf(stderr, "T2 : %s\n\n", treasure.if_no_energy_card);
    }
        

    SDL_Event e;
    int clicked = 0;
    char first_treasure[60] = "cards/sprites/";
    char second_treasure[60] = "cards/sprites/";

    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
    renderCombatText("Click to open the chest", 430, gScreenSurface->h - 200, 32);
    renderMap(load_Pathed_Media("others/chest", 1.5), 490, gScreenSurface->h / 5, 0, 0);
    SDL_UpdateWindowSurface(gWindow);

    FadeEffect(0, 0);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                if (display_errors_on)
                    fprintf(stderr, "\n\nQuiting ...\n\n");
                close_SDL();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = 1;
            }
        }
    }

    clicked = 0;
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
    renderCombatText("Click again to see the rewards", 390, gScreenSurface->h - 200, 32);
    renderMap(load_Pathed_Media("others/chest_open", 1.6), 480, gScreenSurface->h / 5, 0, 0);
    SDL_UpdateWindowSurface(gWindow);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                if (display_errors_on)
                    fprintf(stderr, "\n\nQuiting ...\n\n");
                close_SDL();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = 1;
            }
        }
    }

    clicked = 0;
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

    if (treasure.energy == 1)
    {
        strcpy(first_treasure, "others/mana");
        renderMap(load_Pathed_Media(first_treasure, 0.8), 370, gScreenSurface->h / 3 + 50, 0, 0);
        renderCombatText("1", 490, gScreenSurface->h / 3 + 160, 64);
    }
    else
    {
        strcat(first_treasure, treasure.if_no_energy_card);
        renderMap(load_Pathed_Media(first_treasure, 0.3), 310, gScreenSurface->h / 3 - 40, 0, 0);
    }
    strcat(second_treasure, treasure.card);
    renderMap(load_Pathed_Media(second_treasure, 0.3), 710, gScreenSurface->h / 3 - 40, 0, 0);

    SDL_UpdateWindowSurface(gWindow);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                if (display_errors_on)
                    fprintf(stderr, "\n\nQuiting ...\n\n");
                close_SDL();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = 1;
            }
        }
    }

    FadeEffect(0, 1);
}
