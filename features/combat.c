#include "../headers/include.h"
#include "../headers/combat.h"

void combat(char *map)
{
    struct Player player;
    player = get_player();

    struct Monster monster;
    monster = get_monster(map);

    display_all(player, monster);
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
    int tab[4];
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
    ////////////////////////////// WIP //////////////////////////////
    char token[30];
    if (strpbrk(map, "Elite") != false)
    {
        strcpy(token, "Elite/");
    }
    else if (strpbrk(map, "Common") != false)
    {
        strcpy(token, "Common/");
    }
    else if (strpbrk(map, "Boss") != false)
    {
        strcpy(token, "Boss/");
    }
    else
    {
        printf("An error occured with the Syntax of the monster Name!\n");
        close();
    }
    /////////////////////////////////////////////////////////////////

    struct Monster monster;

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

    printf("\n\nDeck:\n");

    for (int i = 0; i < player.Deck_size; i++)
    {
        printf("------------------ Card : %d ------------------\n", i + 1);
        printf("Name: %s\n", player.Deck[i].name);
        // printf("Energy Cost: %d\n", player.Deck[i].energyCost);
        // printf("Animation: %d\n", player.Deck[i].animation);
        // printf("Description: %s\n", player.Deck[i].description);
        // printf("Damage: %d\n", player.Deck[i].damage);
        // printf("Strength: %d\n", player.Deck[i].strength);
        // printf("Weakness: %d\n", player.Deck[i].weakness);
        // printf("Vulnerability: %d\n", player.Deck[i].vulnerability);
        // printf("Heal: %d\n", player.Deck[i].heal);
        // printf("Armor: %d\n", player.Deck[i].armor);
        // printf("Dodge: %d\n", player.Deck[i].dodge);
        printf("-----------------------------------------------\n");
        printf("\n");
    }
}
