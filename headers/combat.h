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
    int vulnerability;
    int armor;
    int dodge;
    struct Card first_skill;
    struct Card second_skill;
    int second_cd;
    int second_cd_max;
    struct Card third_skill;
    int third_cd;
    int third_cd_max;
};

struct Player get_player();
struct Monster get_monster(char *map);
struct Card get_card(char card_name[30]);
SDL_Surface *load_Pathed_Media(char *full_path, float scale);
SDL_Surface *load_Monster_Media(struct Monster monster);
void display_background(SDL_Surface *background);
void display_actors(SDL_Surface *player_surface, SDL_Surface *monster_surface);
void display_hp_bars(struct Player player, struct Monster monster);
void renderStatus(const char *text, int value, int x);
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
void ennemy_action(int turns, struct Player *player, struct Monster *monster, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface, SDL_Surface *copied_surface_cards);
void update_status(struct Player *player, struct Monster *monster);
void combat_animation(struct Card *card, SDL_Surface *player_surface, SDL_Surface *monster_surface, SDL_Surface *copied_surface, int is_player);
void Cards_Fade(struct Player player, SDL_Rect cardDisplayRects[], int cardsToDisplay, int CurrentCardIndices[], SDL_Surface *copied_surface, int FadeType);
void combat_won();
void combat_lost();
#endif