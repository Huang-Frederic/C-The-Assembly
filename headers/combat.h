#ifndef COMBAT
#define COMBAT

struct Card
{
    char name[30];
    int energyCost;
    int animation;
    char description[150];
    int damage;
    bool strength;
    bool weakness;
    bool vulnerability;
    int heal;
    int armor;
    int dodge;
};

struct Player
{
    int health;
    int maxHealth;
    int energy;
    int maxEnergy;
    int strength;
    int weakness;
    int vulnerability;
    int armor;
    int dodge;
    int Deck_size;
    struct Card *Deck;
};

struct Monster
{
    char name[30];
    int health;
    int maxHealth;
    struct Card first_skill;
    struct Card second_skill;
    int second_cd;
    struct Card third_skill;
    int third_cd;
};

struct Player get_player();
struct Monster get_monster(char *map);
struct Card get_card(char card_name[30]);
void display_all(struct Player player, struct Monster monster);

#endif