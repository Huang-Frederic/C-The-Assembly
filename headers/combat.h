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
    char class[30];
    int health;
    int maxHealth;
    int strength;
    int weakness;
    int vulnerability;
    int armor;
    int dodge;
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
SDL_Surface *load_Pathed_Media(char *full_path, float scale);
SDL_Surface *load_Monster_Media(struct Monster monster);
void display_background(SDL_Surface *background);
void display_actors(SDL_Surface *player_surface, SDL_Surface *monster_surface);
void display_hp_bars(struct Player player, struct Monster monster);
void renderCombatText(const char *text, int x, int y, int font_size);
void display_turns(int turn);
void display_energy_bar(struct Player player);
bool is_in_rect(int x, int y, SDL_Rect rect);
void render_end_turn_button(SDL_Rect endTurnButtonRect, int color);
void display_cards(struct Player player, SDL_Rect cardDisplayRects[], int cardsToDisplay, int CurrentCardIndices[], int hoveredCardIndex);
void initializeCardDisplayRects(SDL_Rect cardDisplayRects[], int cardsToDisplay);
void renderCard(struct Card card, int x, int y, float scale);
void remove_card_from_display(int CurrentCardIndices[], int index, int cardsToDisplay);
bool player_action(struct Player *player, struct Monster *monster, struct Card card, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface);
void ennemy_action();
void combat_animation(struct Card *card, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface, int is_player);
void combat_won();
#endif