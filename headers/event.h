#ifndef EVENT
#define EVENT
struct Save
{
    int deck_size;
    int day;
    char player_name[30];
    int difficulty;
    int hp;
    int max_hp;
    int max_energy;
    int score;
};

struct Treasure
{
    int energy;
    char *card;
    char *if_no_energy_card;
};

void event(char *map);
void display_camp();
void save_camp();
struct Treasure get_treasure();
char *get_treasure_card();
void save_treasure(struct Treasure treasure);
void display_treasure(struct Treasure treasure);
#endif