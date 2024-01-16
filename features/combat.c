#include "../headers/include.h"
#include "../headers/combat.h"

void combat(char *map)
{
    srand(time(NULL));
    // INIT THINGS
    struct Player player;
    struct Monster monster;

    player = get_player();
    monster = get_monster(map);

    SDL_Surface *background = load_Pathed_Media("maps/background2", 1);
    SDL_Surface *player_surface = load_Pathed_Media("Player", 4);
    SDL_Surface *monster_surface = load_Monster_Media(monster);
    SDL_Surface *copiedSurface = SDL_CreateRGBSurface(0, gScreenSurface->w, gScreenSurface->h,
                                                      gScreenSurface->format->BitsPerPixel,
                                                      gScreenSurface->format->Rmask,
                                                      gScreenSurface->format->Gmask,
                                                      gScreenSurface->format->Bmask,
                                                      gScreenSurface->format->Amask);

    display_all(player, monster);

    // For the End of Turn Button
    int end_button_color = 0;
    SDL_Rect endTurnButtonRect = {20, 590, 150, 50};
    bool endTurnButtonClicked = false;
    // For the Card Display
    SDL_Rect cardDisplayRects[5];
    int cardsToDisplay = 5;
    int CurrentCardIndices[] = {-1, -1, -1, -1, -1};
    int hoveredCardIndex = -1;

    int turn = 0;
    int combat_win = 0;
    int player_turn = 0;
    bool end_loop = false;
    SDL_Event e;
    // INIT THINGS END

    initializeCardDisplayRects(cardDisplayRects, cardsToDisplay);

    while (!end_loop)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                printf("\n\nQuiting ...\n\n");
                close();
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (is_in_rect(mouseX, mouseY, endTurnButtonRect))
                {
                    end_button_color = 1;
                }
                else
                {
                    end_button_color = 0;
                }

                for (int i = 0; i < cardsToDisplay; ++i)
                {
                    if (is_in_rect(mouseX, mouseY, cardDisplayRects[i]))
                    {
                        hoveredCardIndex = i;
                        break;
                    }
                    else
                    {
                        hoveredCardIndex = -1;
                    }
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (is_in_rect(mouseX, mouseY, endTurnButtonRect))
                {
                    endTurnButtonClicked = true;
                    end_button_color = 2;
                }
                for (int i = 0; i < cardsToDisplay; ++i)
                {
                    if (is_in_rect(mouseX, mouseY, cardDisplayRects[i]))
                    {
                        if (player_action(&player, &monster, player.Deck[CurrentCardIndices[i]], player_surface, monster_surface, copiedSurface))
                        {
                            // remove the card
                            remove_card_from_display(CurrentCardIndices, i, cardsToDisplay);
                            --cardsToDisplay;
                        }
                    }
                }
            }
        }
        // Clear the screen
        SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

        // Display things
        display_background(background);
        display_hp_bars(player, monster);
        display_turns(turn);
        display_energy_bar(player);
        render_end_turn_button(endTurnButtonRect, end_button_color);
        display_cards(player, cardDisplayRects, cardsToDisplay, CurrentCardIndices, hoveredCardIndex);
        // Create a copy of the original surface
        SDL_BlitSurface(gScreenSurface, NULL, copiedSurface, NULL);
        display_actors(player_surface, monster_surface);

        // Update the window surface
        SDL_UpdateWindowSurface(gWindow);

        if (player_turn == 0)
        {
            renderCombatText("Player Turn", gScreenSurface->w / 3 + 110, gScreenSurface->h / 5, 32);
            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(500);
            player_turn++;
        }
        else
        {
            SDL_UpdateWindowSurface(gWindow);
        }

        if (endTurnButtonClicked)
        {
            ennemy_action();
            endTurnButtonClicked = false;
            end_button_color = 0;
            turn++;
            player_turn = 0;
            cardsToDisplay = 5;
            CurrentCardIndices[0] = -1;
            player.energy = player.maxEnergy;
        }

        if (monster.health <= 0)
        {
            combat_win = 1;
            end_loop = true;
        }
        else if (player.health <= 0)
        {
            combat_win = 0;
            end_loop = true;
        }
    }

    if (combat_win == 1)
    {
        combat_won();
    }
    else
    {
        printf("YOU LOST\n");
    }
    close();
}

struct Player get_player()
{
    struct Player player;

    // use fscanf to read save.txt file
    FILE *save = fopen("save.txt", "r");
    if (save == NULL)
    {
        printf("An error while opening save.txt have occured!\n");
        close();
    }

    // Set all informations except Deck
    int tab[5];
    for (int i = 0; i < 5; i++)
    {
        fscanf(save, "%d", &tab[i]);
    }
    player.Deck_size = tab[0];
    player.health = tab[2];
    player.maxHealth = tab[3];
    player.energy = tab[4];
    player.maxEnergy = tab[4];
    player.strength = 0;
    player.weakness = 0;
    player.vulnerability = 0;
    player.armor = 0;
    player.dodge = 0;

    // Set Deck
    player.Deck = malloc(player.Deck_size * sizeof(struct Card));
    char deck[player.Deck_size][30];
    for (int i = 0; i < player.Deck_size; i++)
    {
        fscanf(save, "%s", deck[i]);
        player.Deck[i] = get_card(deck[i]);
    }

    return player;
}

struct Monster get_monster(char *map)
{
    struct Monster monster;

    ////////////////////////////// WIP //////////////////////////////
    char token[30];
    if (strpbrk(map, "Elite") != false)
    {
        strcpy(token, "Elite/");
        strcpy(monster.class, "Elite");
    }
    else if (strpbrk(map, "Common") != false)
    {
        strcpy(token, "Common/");
        strcpy(monster.class, "Common");
    }
    else if (strpbrk(map, "Boss") != false)
    {
        strcpy(token, "Boss/");
        strcpy(monster.name, "Boss");
    }
    else
    {
        printf("An error occured with the Syntax of the monster Name!\n");
        close();
    }
    /////////////////////////////////////////////////////////////////

    char monster_name[50];
    strcpy(monster_name, map);
    strcat(monster_name, ".txt");
    char path[50] = "data/monsters/";
    //////////////////////////////////
    strcat(path, token);
    //////////////////////////////////
    strcat(path, monster_name);

    FILE *monster_file = fopen(path, "r");
    if (monster_file == NULL)
    {
        printf("An error while opening monster_file.txt have occured!\n");
        close();
    }
    else
    {
        // Set all informations except Deck
        fscanf(monster_file, "%s", monster.name);
        fscanf(monster_file, "%d", &monster.health);
        monster.maxHealth = monster.health;
        monster.strength = 0;
        monster.weakness = 0;
        monster.vulnerability = 0;
        monster.armor = 0;
        monster.dodge = 0;

        char first_skill[30];
        fscanf(monster_file, "%s", first_skill);
        monster.first_skill = get_card(first_skill);

        char second_skill[30];
        fscanf(monster_file, "%s", second_skill);
        monster.second_skill = get_card(second_skill);
        fscanf(monster_file, "%d", &monster.second_cd);

        char third_skill[30];
        if (fscanf(monster_file, "%s", third_skill) != EOF)
        {
            monster.third_skill = get_card(third_skill);
            fscanf(monster_file, "%d", &monster.third_cd);
        }
        else
        {
            strcpy(third_skill, "None");
            monster.third_skill = get_card(third_skill);
            monster.third_cd = 0;
        }
    }

    fclose(monster_file);
    return monster;
}

struct Card get_card(char card_name[30])
{
    struct Card card;

    if (strcmp(card_name, "None") == 0)
    {
        strcpy(card.name, "None");
        card.energyCost = 0;
        card.animation = 0;
        strcpy(card.description, "None");
        card.damage = 0;
        card.strength = 0;
        card.weakness = 0;
        card.vulnerability = 0;
        card.heal = 0;
        card.armor = 0;
        card.dodge = 0;
        return card;
    }

    // use fscanf to read the card_name file
    strcat(card_name, ".txt");
    char path[50] = "data/cards/";
    strcat(path, card_name);

    FILE *card_file = fopen(path, "r");
    if (card_file == NULL)
    {
        printf("An error while opening %s have occured!\n", card_name);
        close();
    }

    for (int i = 0; i < 11; i++)
    {
        fscanf(card_file, "%s", card.name);
        fscanf(card_file, "%d", &card.energyCost);
        fscanf(card_file, "%d", &card.animation);
        fscanf(card_file, " %[^\n]", card.description);
        fscanf(card_file, "%d", &card.damage);
        fscanf(card_file, "%d", &card.strength);
        fscanf(card_file, "%d", &card.weakness);
        fscanf(card_file, "%d", &card.vulnerability);
        fscanf(card_file, "%d", &card.heal);
        fscanf(card_file, "%d", &card.armor);
        fscanf(card_file, "%d", &card.dodge);
    }

    fclose(card_file); // Close the file after reading

    return card;
}

void display_all(struct Player player, struct Monster monster)
{
    printf("----------------------------------------\n");
    printf("Health: %d/%d\n", player.health, player.maxHealth);
    printf("Energy: %d/%d\n", player.energy, player.maxEnergy);
    printf("Strength: %d\n", player.strength);
    printf("Weakness: %d\n", player.weakness);
    printf("Vulnerability: %d\n", player.vulnerability);
    printf("Armor: %d\n", player.armor);
    printf("Dodge: %d\n", player.dodge);
    printf("Deck Size: %d\n", player.Deck_size);
    printf("----------------------------------------\n");

    // print all
    printf("----------------------------------------\n");
    printf("Name: %s\n", monster.name);
    printf("Health: %d/%d\n", monster.health, monster.maxHealth);
    printf("First Skill: %s\n", monster.first_skill);
    printf("Second Skill: %s\n", monster.second_skill);
    printf("Second Skill CD: %d\n", monster.second_cd);
    printf("Third Skill: %s\n", monster.third_skill);
    printf("Third Skill CD: %d\n", monster.third_cd);
    printf("----------------------------------------\n");

    // printf("\n\nDeck:\n");

    // for (int i = 0; i < player.Deck_size; i++)
    // {
    //     printf("------------------ Card : %d ------------------\n", i + 1);
    //     printf("Name: %s\n", player.Deck[i].name);
    //     printf("Energy Cost: %d\n", player.Deck[i].energyCost);
    //     printf("Animation: %d\n", player.Deck[i].animation);
    //     printf("Description: %s\n", player.Deck[i].description);
    //     printf("Damage: %d\n", player.Deck[i].damage);
    //     printf("Strength: %d\n", player.Deck[i].strength);
    //     printf("Weakness: %d\n", player.Deck[i].weakness);
    //     printf("Vulnerability: %d\n", player.Deck[i].vulnerability);
    //     printf("Heal: %d\n", player.Deck[i].heal);
    //     printf("Armor: %d\n", player.Deck[i].armor);
    //     printf("Dodge: %d\n", player.Deck[i].dodge);
    //     printf("-----------------------------------------------\n");
    //     printf("\n");
    // }
}

SDL_Surface *load_Pathed_Media(char *path, float scale)
{
    char full_path[255] = "data/";
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

    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(originalSurface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == NULL)
    {
        printf("Unable to convert surface! SDL Error: %s\n", SDL_GetError());
        close();
    }

    int desiredWidth = formattedSurface->w * scale;
    int desiredHeight = formattedSurface->h * scale;

    SDL_Surface *scaledSurface = SDL_CreateRGBSurfaceWithFormat(0, desiredWidth, desiredHeight, 32, SDL_PIXELFORMAT_RGBA32);

    if (scaledSurface == NULL)
    {
        printf("Unable to scale surface! SDL Error: %s\n", SDL_GetError());
        close();
    }

    SDL_BlitScaled(formattedSurface, NULL, scaledSurface, NULL);

    SDL_FreeSurface(originalSurface);
    SDL_FreeSurface(formattedSurface);

    return scaledSurface;
}

SDL_Surface *load_Monster_Media(struct Monster monster)
{
    char monster_path[256] = "monsters/";
    strcat(monster_path, monster.class);
    strcat(monster_path, "/Sprites/");
    strcat(monster_path, monster.name);
    return load_Pathed_Media(monster_path, 4);
}

void display_background(SDL_Surface *background)
{
    renderMap(background, 0, 0, 0, 0);
}

void display_actors(SDL_Surface *player_surface, SDL_Surface *monster_surface)
{
    int screenWidth = gScreenSurface->w;
    // Calculate positions to face each other
    int surface1X = (screenWidth - player_surface->w) / 4;
    int surface2X = (screenWidth - monster_surface->w) / 4 * 3;
    int surfaceY = (gScreenSurface->h - player_surface->h) / 2;

    // Render the surfaces
    renderMap(player_surface, surface1X, surfaceY, 0, 0);
    renderMap(monster_surface, surface2X, surfaceY, 0, 0);
}

void display_hp_bars(struct Player player, struct Monster monster)
{
    int playerHpBarWidth = (int)((float)player.health / player.maxHealth * 500);
    int monsterHpBarWidth = (int)((float)monster.health / monster.maxHealth * 500);

    // Draw underlayer
    SDL_Rect underlayerPlayer = {30, 45, 500, 30};
    SDL_Rect underlayerMonster = {gScreenSurface->w - 530, 45, 500, 30};
    SDL_FillRect(gScreenSurface, &underlayerPlayer, SDL_MapRGB(gScreenSurface->format, 150, 150, 150));
    SDL_FillRect(gScreenSurface, &underlayerMonster, SDL_MapRGB(gScreenSurface->format, 150, 150, 150));

    // Draw player's HP bar
    SDL_Rect playerHpRect = {30, 45, playerHpBarWidth, 30};
    SDL_FillRect(gScreenSurface, &playerHpRect, SDL_MapRGB(gScreenSurface->format, 0, 255, 0));

    // Draw monster's HP bar
    SDL_Rect monsterHpRect = {gScreenSurface->w - monsterHpBarWidth - 30, 45, monsterHpBarWidth, 30};
    SDL_FillRect(gScreenSurface, &monsterHpRect, SDL_MapRGB(gScreenSurface->format, 255, 0, 0));

    // Display HP values
    char playerHpText[20];
    char monsterHpText[20];
    sprintf(playerHpText, "Player HP: %d/%d", player.health, player.maxHealth);
    sprintf(monsterHpText, "Monster HP: %d/%d", monster.health, monster.maxHealth);

    renderCombatText(playerHpText, 30, 15, 24);
    renderCombatText(monsterHpText, gScreenSurface->w - 265, 15, 24);
}

void renderCombatText(const char *text, int x, int y, int font_size)
{
    loadFont(selectedFont, font_size);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *message = TTF_RenderText_Solid(font, text, textColor);
    if (message != NULL)
    {
        SDL_Rect textRect = {x, y, 0, 0};
        SDL_BlitSurface(message, NULL, gScreenSurface, &textRect);
        SDL_FreeSurface(message);
    }
}

void display_turns(int turn)
{
    char turnText[20];
    sprintf(turnText, "%d", turn);
    renderCombatText("Round", gScreenSurface->w / 2 - 75, 10, 52);
    renderCombatText(turnText, gScreenSurface->w / 2 - 15, 50, 52);
}

void display_energy_bar(struct Player player)
{
    int playerEnergy = player.energy;
    int maxPlayerEnergy = player.maxEnergy;
    int orbSize = 20; // Adjust the size of each orb as needed
    int spacing = 5;  // Adjust the spacing between orbs as needed

    // Display blue orbs for available energy
    for (int i = 0; i < playerEnergy; ++i)
    {
        SDL_Rect orbRect = {20 + i * (orbSize + spacing), gScreenSurface->h - 40, orbSize, orbSize};
        SDL_FillRect(gScreenSurface, &orbRect, SDL_MapRGB(gScreenSurface->format, 0, 0, 255)); // Blue color
    }

    // Display grey orbs for used energy
    for (int i = playerEnergy; i < maxPlayerEnergy; ++i)
    {
        SDL_Rect orbRect = {20 + i * (orbSize + spacing), gScreenSurface->h - 40, orbSize, orbSize};
        SDL_FillRect(gScreenSurface, &orbRect, SDL_MapRGB(gScreenSurface->format, 128, 128, 128)); // Grey color
    }

    // Display numerical energy value
    char energyText[20];
    sprintf(energyText, "Energy: %d/%d", playerEnergy, maxPlayerEnergy);
    renderCombatText(energyText, 20, gScreenSurface->h - 70, 24); // Adjust the position as needed
}

bool is_in_rect(int x, int y, SDL_Rect rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

void render_end_turn_button(SDL_Rect endTurnButtonRect, int color)
{
    switch (color)
    {
    case 0:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 210, 210, 0)); // Red color
        break;
    case 1:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 100, 230, 100)); // Green color
        break;
    case 2:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 128, 128, 128)); // Blue color
        break;
    default:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 255, 255, 255)); // White color
        break;
    }
    renderCombatText("End of Turn", endTurnButtonRect.x + 10, endTurnButtonRect.y + 15, 21); // Adjust the text position
}

void display_cards(struct Player player, SDL_Rect cardDisplayRects[], int cardsToDisplay, int CurrentCardIndices[], int hoveredCardIndex)
{
    if (CurrentCardIndices[0] == -1)
    {
        for (int i = 0; i < cardsToDisplay; ++i)
        {
            CurrentCardIndices[i] = rand() % player.Deck_size;
            struct Card randomCard = player.Deck[CurrentCardIndices[i]];
            renderCard(randomCard, cardDisplayRects[i].x, cardDisplayRects[i].y, 0.25);
        }
    }
    else if (CurrentCardIndices[0] != -1)
    {
        for (int i = 0; i < cardsToDisplay; ++i)
        {
            if (hoveredCardIndex == i)
            {
                renderCard(player.Deck[CurrentCardIndices[i]], cardDisplayRects[i].x, cardDisplayRects[i].y - 50, 0.25);

                renderCard(player.Deck[CurrentCardIndices[i]], 10, gScreenSurface->h / 4 + 10, 0.35);
            }
            else
            {
                renderCard(player.Deck[CurrentCardIndices[i]], cardDisplayRects[i].x, cardDisplayRects[i].y, 0.25);
            }
        }
    }
}

void initializeCardDisplayRects(SDL_Rect cardDisplayRects[], int cardsToDisplay)
{
    int cardWidth = 100;
    int cardHeight = 275;
    int padding = 0;

    int totalWidth = (cardWidth + padding) * cardsToDisplay - padding;
    int initialX = gScreenSurface->w - totalWidth - 450;

    for (int i = 0; i < cardsToDisplay; ++i)
    {
        cardDisplayRects[i].x = initialX + i * (cardWidth + padding);
        cardDisplayRects[i].y = gScreenSurface->h - cardHeight + 100;
        cardDisplayRects[i].w = cardWidth;
        cardDisplayRects[i].h = cardHeight;
    }
}

void renderCard(struct Card card, int x, int y, float scale)

{
    char imagePath[100];
    sprintf(imagePath, "cards/Sprites/%s", card.name);

    SDL_Surface *cardImage = load_Pathed_Media(imagePath, scale);
    if (cardImage == NULL)
    {
        printf("Failed to load card image: %s\n", imagePath);
        close();
    }

    SDL_Rect destRect = {x, y, cardImage->w, cardImage->h};
    SDL_BlitSurface(cardImage, NULL, gScreenSurface, &destRect);
    SDL_FreeSurface(cardImage);
}

void remove_card_from_display(int CurrentCardIndices[], int index, int cardsToDisplay)
{
    int tab[cardsToDisplay];
    for (int j = 0; j < index; ++j)
    {
        tab[j] = CurrentCardIndices[j];
    }
    for (int j = index; j < cardsToDisplay - 1; ++j)
    {
        tab[j] = CurrentCardIndices[j + 1];
    }
    for (int j = 0; j < cardsToDisplay - 1; ++j)
    {
        CurrentCardIndices[j] = tab[j];
    }
}

bool player_action(struct Player *player, struct Monster *monster, struct Card card, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface)
{
    char message[100];
    if (player->energy >= card.energyCost)
    {
        // Anounce the card played
        sprintf(message, "You have played %s", card.name);
        renderCombatText(message, gScreenSurface->w / 3, gScreenSurface->h / 5, 32);
        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay(1 * 400);

        // Apply the card effects on player
        player->health += card.heal;
        player->armor += card.armor;
        player->dodge += card.dodge;
        player->energy -= card.energyCost;
        player->strength = card.strength ? 3 : 0;

        // apply multipliers
        card.damage = player->strength != 0 ? card.damage * 1.25 : card.damage;
        int vulnerability = monster->vulnerability != 0 ? 1.5 : 1;

        // if monster dodged
        if (monster->dodge > 0)
        {
            srand(time(NULL));
            int dodge = rand() % 100;
            switch (dodge)
            {
            case 0 ... 75:
                while (monster->armor != 0 && card.damage != 0)
                {
                    card.damage -= 1;
                    monster->armor -= 1 * vulnerability;
                }
                while (monster->health != 0 && card.damage != 0)
                {
                    card.damage -= 1;
                    monster->health -= 1 * vulnerability;
                }
                break;
            case 76 ... 100:
                printf("The monster dodged the attack!\n");
                sprintf(message, "The monster dodged the attack!");
                break;
            }
        }
        else
        {
            while (monster->armor != 0 && card.damage != 0)
            {
                card.damage -= 1;
                monster->armor -= 1 * vulnerability;
            }
            while (monster->health != 0 && card.damage != 0)
            {
                card.damage -= 1;
                monster->health -= 1 * vulnerability;
            }
        }

        monster->vulnerability = card.vulnerability ? 3 : 0;

        combat_animation(&card, player_surface, monster_surface, copied_surface, 1);
        return true;
    }
    else
    {
        sprintf(message, "You don't have enough energy to play %s", card.name);
        renderCombatText(message, gScreenSurface->w / 5, gScreenSurface->h / 5, 32);
        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay(1 * 500);
        return false;
    }
}

void ennemy_action()
{
    // TODO
    printf("PAUSE STARTED\n");
    renderCombatText("Ennemy Turn", gScreenSurface->w / 3 + 110, gScreenSurface->h / 5, 32);
    SDL_UpdateWindowSurface(gWindow);
    SDL_Delay(500);
    printf("PAUSE ENDED\n");
}

void combat_animation(struct Card *card, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface, int is_player)
{
    int screenWidth = gScreenSurface->w;
    // Calculate positions to face each other
    int surface1X = (screenWidth - player_surface->w) / 4;
    int surface2X = (screenWidth - monster_surface->w) / 4 * 3;
    int surfaceY = (gScreenSurface->h - player_surface->h) / 2;

    if (is_player == 1)
    {
        switch (card->animation)
        {
        case 1:
            for (int i = 1; i < 10; i++)
            {
                renderMap(player_surface, surface1X + (i * 5), surfaceY, 0, 0);
                renderMap(monster_surface, surface2X + (i * 5), surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            for (int i = 10; i > 1; i--)
            {
                renderMap(player_surface, surface1X + (i * 5), surfaceY, 0, 0);
                renderMap(monster_surface, surface2X + (i * 5), surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            break;
        case 2:
            for (int i = 1; i < 10; i++)
            {
                renderMap(player_surface, surface1X, surfaceY - (i * 5), 0, 0);
                renderMap(monster_surface, surface2X, surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            for (int i = 10; i > 1; i--)
            {
                renderMap(player_surface, surface1X, surfaceY - (i * 5), 0, 0);
                renderMap(monster_surface, surface2X, surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            break;
        case 3:
            double angle = 0.0;
            for (int i = 0; i < 32; i++)
            {
                double radians = angle * (M_PI / 180.0);
                int x = surface1X + (int)(50 * cos(radians));
                int y = surfaceY + (int)(50 * sin(radians));

                renderMap(player_surface, x, y, 0, 0);
                renderMap(monster_surface, surface2X, surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
                angle += 10.0; // Adjust the angle step based on your requirements
            }
            break;
        default:
            printf("No animation\n");
            close();
            break;
        }
    }
    else if (is_player == 0)
    {
        switch (card->animation)
        {
        case 1:
            for (int i = 1; i < 10; i++)
            {
                renderMap(player_surface, surface1X - (i * 5), surfaceY, 0, 0);
                renderMap(monster_surface, surface2X - (i * 5), surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            for (int i = 10; i > 1; i--)
            {
                renderMap(player_surface, surface1X - (i * 5), surfaceY, 0, 0);
                renderMap(monster_surface, surface2X - (i * 5), surfaceY, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            break;
        case 2:
            for (int i = 1; i < 10; i++)
            {
                renderMap(player_surface, surface1X, surfaceY, 0, 0);
                renderMap(monster_surface, surface2X, surfaceY - (i * 5), 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            for (int i = 10; i > 1; i--)
            {
                renderMap(player_surface, surface1X, surfaceY, 0, 0);
                renderMap(monster_surface, surface2X, surfaceY - (i * 5), 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
            }
            break;
        case 3:
            double angle = 0.0;
            for (int i = 0; i < 32; i++)
            {
                double radians = angle * (M_PI / 180.0);
                int x = surface2X + (int)(50 * cos(radians));
                int y = surfaceY + (int)(50 * sin(radians));

                renderMap(player_surface, surface1X, surfaceY, 0, 0);
                renderMap(monster_surface, x, y, 0, 0);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(20);
                SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);
                angle += 10.0; // Adjust the angle step based on your requirements
            }
            break;
        default:
            printf("No animation\n");
            close();
            break;
        }
    }
}

void combat_won()
{
    // TODO
    printf("YOU WON\n");
    close();
}

// Afficher les status du joueur et du monstre