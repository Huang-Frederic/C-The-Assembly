// Init Functions
char* map(int day);
SDL_Surface *loadMedia(char *path, int scale);
void renderMap(SDL_Surface *map, int x, int y);
bool isMouseInside(int mouseX, int mouseY, int x_start, SDL_Surface *map);
void renderText(SDL_Surface *surface, const char *text, int x, int y);
void addSpaceBeforeUppercase(char *str);