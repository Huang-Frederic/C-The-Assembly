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
    SDL_Surface *copied_surface_cards = SDL_CreateRGBSurface(0, gScreenSurface->w, gScreenSurface->h,
                                                             gScreenSurface->format->BitsPerPixel,
                                                             gScreenSurface->format->Rmask,
                                                             gScreenSurface->format->Gmask,
                                                             gScreenSurface->format->Bmask,
                                                             gScreenSurface->format->Amask);

    // For the End of Turn Button
    int end_button_color = 0;
    SDL_Rect endTurnButtonRect = {gScreenSurface->w - 225, gScreenSurface->h - 100, 200, 75};
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

                end_button_color = is_in_rect(mouseX, mouseY, endTurnButtonRect) ? 1 : 0;

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
                            // clear PollEvent
                            while (SDL_PollEvent(&e) != 0)
                            {
                            }
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

        // Create a copy of the original surface
        SDL_BlitSurface(gScreenSurface, NULL, copiedSurface, NULL);
        display_actors(player_surface, monster_surface);

        // Create a copy of the original surface
        SDL_BlitSurface(gScreenSurface, NULL, copied_surface_cards, NULL);
        display_cards(player, cardDisplayRects, cardsToDisplay, CurrentCardIndices, hoveredCardIndex);

        if (player_turn == 0)
        {
            Cards_Fade(player, cardDisplayRects, cardsToDisplay, CurrentCardIndices, copied_surface_cards, 1);
            renderCombatText("Player Turn", gScreenSurface->w / 3 - 40, gScreenSurface->h / 5 - 10, 84);
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
            Cards_Fade(player, cardDisplayRects, cardsToDisplay, CurrentCardIndices, copied_surface_cards, 0);
            ennemy_action(turn, &player, &monster, player_surface, monster_surface, copiedSurface, copied_surface_cards);
            endTurnButtonClicked = false;
            end_button_color = 0;
            turn++;
            player_turn = 0;
            cardsToDisplay = 5;
            CurrentCardIndices[0] = -1;
            player.energy = player.maxEnergy;
            update_status(&player, &monster);
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

    combat_win == 1 ? combat_won() : combat_lost();
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
    strcat(path, token);
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
        monster.vulnerability = 0;
        monster.armor = 0;
        monster.dodge = 0;

        char first_skill[30];
        fscanf(monster_file, "%s", first_skill);
        monster.first_skill = get_card(first_skill);

        char second_skill[30];
        fscanf(monster_file, "%s", second_skill);
        monster.second_skill = get_card(second_skill);
        fscanf(monster_file, "%d", &monster.second_cd_max);
        monster.second_cd = -1;

        char third_skill[30];
        if (fscanf(monster_file, "%s", third_skill) != EOF)
        {
            monster.third_skill = get_card(third_skill);
            fscanf(monster_file, "%d", &monster.third_cd_max);
            monster.third_cd = -1;
        }
        else
        {
            strcpy(third_skill, "None");
            monster.third_skill = get_card(third_skill);
            monster.third_cd = 0;
            monster.third_cd_max = 0;
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

    for (int i = 0; i < 10; i++)
    {
        fscanf(card_file, "%s", card.name);
        fscanf(card_file, "%d", &card.energyCost);
        fscanf(card_file, "%d", &card.animation);
        fscanf(card_file, " %[^\n]", card.description);
        fscanf(card_file, "%d", &card.damage);
        fscanf(card_file, "%d", &card.strength);
        fscanf(card_file, "%d", &card.vulnerability);
        fscanf(card_file, "%d", &card.heal);
        fscanf(card_file, "%d", &card.armor);
        fscanf(card_file, "%d", &card.dodge);
    }

    fclose(card_file); // Close the file after reading

    return card;
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

    // Display HP values Player
    char playerHpText[20];
    sprintf(playerHpText, "Player HP: %d/%d", player.health, player.maxHealth);
    renderCombatText(playerHpText, 30, 15, 24);

    // Display HP values Monster
    char monsterHpText[20];
    sprintf(monsterHpText, "Monster HP: %d/%d", monster.health, monster.maxHealth);
    renderCombatText(monsterHpText, gScreenSurface->w - 265, 15, 24);

    // Display Status
    int playerStatusX = 30;
    int monsterStatusX = gScreenSurface->w - 530;
    int spaces = 50;

    player.armor != 0 ? (renderStatus("Armor", player.armor, playerStatusX), playerStatusX += spaces + 10) : 0;
    player.strength != 0 ? (renderStatus("Strength", player.strength, playerStatusX), playerStatusX += spaces) : 0;
    player.vulnerability != 0 ? (renderStatus("Vulnerability", player.vulnerability, playerStatusX), playerStatusX += spaces) : 0;
    player.dodge != 0 ? (renderStatus("Dodge", player.dodge, playerStatusX), playerStatusX += spaces) : 0;

    monster.armor != 0 ? (renderStatus("Armor", monster.armor, monsterStatusX), monsterStatusX += spaces + 10) : 0;
    monster.strength != 0 ? (renderStatus("Strength", monster.strength, monsterStatusX), monsterStatusX += spaces) : 0;
    monster.vulnerability != 0 ? (renderStatus("Vulnerability", monster.vulnerability, monsterStatusX), monsterStatusX += spaces) : 0;
    monster.dodge != 0 ? (renderStatus("Dodge", monster.dodge, monsterStatusX), monsterStatusX += spaces) : 0;
}

void renderStatus(const char *text, int value, int x)
{
    char full_path[20];
    sprintf(full_path, "effects/%s", text);

    SDL_Surface *statusImage = load_Pathed_Media(full_path, 0.15);
    if (statusImage == NULL)
    {
        printf("Failed to load status image: %s\n", full_path);
        close();
    }

    char valueString[5];
    sprintf(valueString, "%d", value);
    renderMap(statusImage, x, 85, 0, 0);
    renderCombatText(valueString, x + 30, 95, 24);
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
    sprintf(turnText, "%d", turn + 1);
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
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 54, 82, 173)); // Nothing
        break;
    case 1:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 40, 32, 150)); // Hovered
        break;
    case 2:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 128, 128, 128)); // Clicked
        break;
    default:
        SDL_FillRect(gScreenSurface, &endTurnButtonRect, SDL_MapRGB(gScreenSurface->format, 255, 255, 255)); // White color
        break;
    }
    renderCombatText("End of Turn", endTurnButtonRect.x + 30, endTurnButtonRect.y + 25, 24); // Adjust the text position
}

void display_cards(struct Player player, SDL_Rect cardDisplayRects[], int cardsToDisplay, int CurrentCardIndices[], int hoveredCardIndex)
{
    if (CurrentCardIndices[0] == -1)
    {
        for (int i = 0; i < cardsToDisplay; ++i)
        {
            CurrentCardIndices[i] = rand() % player.Deck_size;
            int isTheSame = 0;
            for (int j = 0; j < cardsToDisplay; j++)
            {
                if (CurrentCardIndices[i] == CurrentCardIndices[j] && i != j)
                {
                    isTheSame = 1;
                }
            }
            if (isTheSame == 1)
            {
                --i;
            }
            else
            {
                struct Card randomCard = player.Deck[CurrentCardIndices[i]];
                renderCard(randomCard, cardDisplayRects[i].x, cardDisplayRects[i].y, 0.25);
            }
        }
    }
    else if (CurrentCardIndices[0] != -1)
    {
        for (int i = 0; i < cardsToDisplay; ++i)
        {
            if (hoveredCardIndex == i)
            {
                renderCard(player.Deck[CurrentCardIndices[i]], cardDisplayRects[i].x, cardDisplayRects[i].y - 50, 0.25);
                renderCard(player.Deck[CurrentCardIndices[i]], 10, gScreenSurface->h / 4 - 30, 0.35);
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
    int cardWidth = 110;
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

        // apply multipliers
        card.damage = player->strength != 0 ? card.damage * 1.25 : card.damage;
        int vulnerability = monster->vulnerability != 0 ? 1.5 : 1;

        // if monster dodged then apply dammages in function of armor and vulnerability
        // if player dodged then apply dammages in function of armor and vulnerability
        if (card.damage != 0)
        {
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
        }

        // Apply the card effects
        player->health += card.heal;
        player->armor += card.armor;
        player->dodge = card.dodge ? 3 : player->dodge;
        player->energy -= card.energyCost;
        player->strength = card.strength ? 3 : player->strength;
        monster->vulnerability = card.vulnerability ? 3 : monster->vulnerability;

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

void ennemy_action(int turns, struct Player *player, struct Monster *monster, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface, SDL_Surface *copied_surface_cards)
{
    // render the text
    renderCombatText("Ennemy Turn", gScreenSurface->w / 3 - 40, gScreenSurface->h / 5 - 10, 84);
    SDL_UpdateWindowSurface(gWindow);
    SDL_Delay(500);

    char message[100];
    struct Card card;

    // select the card
    if (monster->third_cd <= -1 && monster->third_skill.name != "None")
    {
        card = monster->third_skill;
        monster->third_cd = monster->third_cd_max;
    }
    else if (monster->second_cd <= -1)
    {
        card = monster->second_skill;
        monster->second_cd = monster->second_cd_max;
    }
    else
    {
        card = monster->first_skill;
    }

    // render the card and the message
    SDL_BlitSurface(copied_surface_cards, NULL, gScreenSurface, NULL);
    renderCombatText(message, gScreenSurface->w / 3, gScreenSurface->h / 5, 32);
    renderCard(card, gScreenSurface->w - 250, gScreenSurface->h / 3 - 40, 0.3);
    SDL_UpdateWindowSurface(gWindow);
    SDL_Delay(800);

    // apply multipliers
    card.damage = monster->strength != 0 ? card.damage * 1.25 : card.damage;
    int vulnerability = player->vulnerability != 0 ? 1.5 : 1;

    // if player dodged then apply dammages in function of armor and vulnerability
    if (card.damage != 0)
    {
        if (player->dodge > 0)
        {
            srand(time(NULL));
            int dodge = rand() % 100;
            switch (dodge)
            {
            case 0 ... 75:
                while (player->armor != 0 && card.damage != 0)
                {
                    card.damage -= 1;
                    player->armor -= 1 * vulnerability;
                }
                while (player->health != 0 && card.damage != 0)
                {
                    card.damage -= 1;
                    player->health -= 1 * vulnerability;
                }
                break;
            case 76 ... 100:
                printf("You have dodged the attack!\n");
                SDL_BlitSurface(copied_surface_cards, NULL, gScreenSurface, NULL);
                renderCombatText("You have dodged the attack!", gScreenSurface->w / 3, gScreenSurface->h / 5, 32);
                SDL_UpdateWindowSurface(gWindow);
                SDL_Delay(800);
                break;
            }
        }
        else
        {
            while (player->armor != 0 && card.damage != 0)
            {
                card.damage -= 1;
                player->armor -= 1 * vulnerability;
            }
            while (player->health != 0 && card.damage != 0)
            {
                card.damage -= 1;
                player->health -= 1 * vulnerability;
            }
        }
    }

    // Apply the card effects
    monster->health += card.heal;
    monster->armor += card.armor;
    monster->dodge = card.dodge ? 4 : monster->dodge;
    monster->strength = card.strength ? 4 : monster->strength;
    player->vulnerability = card.vulnerability ? 4 : monster->vulnerability;

    // animation
    SDL_BlitSurface(copied_surface_cards, NULL, gScreenSurface, NULL);
    combat_animation(&card, player_surface, monster_surface, copied_surface, 0);

    // reduce cd
    monster->second_cd -= 1;
    monster->third_cd -= 1;
}

void update_status(struct Player *player, struct Monster *monster)
{
    if (player->strength > 0)
    {
        player->strength -= 1;
    }
    if (player->vulnerability > 0)
    {
        player->vulnerability -= 1;
    }
    if (player->dodge > 0)
    {
        player->dodge -= 1;
    }
    if (monster->strength > 0)
    {
        monster->strength -= 1;
    }
    if (monster->vulnerability > 0)
    {
        monster->vulnerability -= 1;
    }
    if (monster->dodge > 0)
    {
        monster->dodge -= 1;
    }
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

void Cards_Fade(struct Player player, SDL_Rect cardDisplayRects[], int cardsToDisplay, int CurrentCardIndices[], SDL_Surface *copied_surface, int FadeType)
{
    SDL_Surface *cardImages[cardsToDisplay];
    for (int i = 0; i < cardsToDisplay; ++i)
    {
        char imagePath[100];
        sprintf(imagePath, "cards/Sprites/%s", player.Deck[CurrentCardIndices[i]].name);

        cardImages[i] = load_Pathed_Media(imagePath, 0.25);
        if (cardImages[i] == NULL)
        {
            printf("Failed to load card image: %s\n", imagePath);
            close();
        }
    }

    if (FadeType == 1)
    {
        for (int j = 0; j <= 40; j++)
        {
            SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);

            for (int i = 0; i < cardsToDisplay; ++i)
            {
                SDL_Rect destRect = {cardDisplayRects[i].x, cardDisplayRects[i].y + 200 - (j * 5), cardImages[i]->w, cardImages[i]->h};
                SDL_BlitSurface(cardImages[i], NULL, gScreenSurface, &destRect);
            }

            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(20);
        }
    }
    else
    {
        for (int j = 40; j >= 0; j--)
        {
            SDL_BlitSurface(copied_surface, NULL, gScreenSurface, NULL);

            for (int i = 0; i < cardsToDisplay; ++i)
            {
                SDL_Rect destRect = {cardDisplayRects[i].x, cardDisplayRects[i].y + 200 - (j * 5), cardImages[i]->w, cardImages[i]->h};
                SDL_BlitSurface(cardImages[i], NULL, gScreenSurface, &destRect);
            }

            SDL_UpdateWindowSurface(gWindow);
            SDL_Delay(20);
        }
    }

    for (int i = 0; i < cardsToDisplay; ++i)
    {
        SDL_FreeSurface(cardImages[i]);
    }
}

void combat_won()
{
    // TODO Récompenses
    printf("YOU WON\n");
    close();
}

void combat_lost()
{
    // TODO Score
    printf("YOU LOST\n");
    close();
}

// Combat Lost -> Si on perd affiche le score et renvoi au menu et ensuite supprimer la save
// Combat Won -> Si on gagne offre les récompenses (1:HP, 2:Carte, 3:Carte/Energy) et sauvegarde (update le score, applique les récompenses), passe a la prochaine map
// Fade in fade out de la scene de combat (Affiche le message de Curl avant de fade in)
// Background qui varie en fonction du pays
// Gérer la difficulté, ajouter dans save la difficuté, qui sera écrite dans la save, PV +20%, Buff ATK, Buff Dodge au début de partie.
// Faire un guide au lancement de la partie

// Changer les trucs et commecner à mettre de vrais cartes/monstres
