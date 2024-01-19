// Init Functions
char *map();
SDL_Surface *load_Pathed_Media_Map(char *path, float scale);
SDL_Surface *load_Background_Media_Map(char *background_image);
SDL_Surface *loadMedia(char *path, float scale);
void renderMap(SDL_Surface *map, int x, int y, int w, int h);
void renderMapText(SDL_Surface *surface, char *Text, int font_size, float width_divide, float height_divide);
int generateMaps(int day, char map_choices[][20], char countries[][20], int map_occurrence, char map_chosen[][20], SDL_Surface *maps[]);
bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map);
void renderTextChosen(SDL_Surface *surface, const char *text, int x, int y);
void addSpaceBeforeUppercase(char *str);
int FadeEffect(int faded, int inOut);
int get_save_day();
