
#ifndef DEFAULT
#define DEFAULT

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const char *selectedFont = "upheavtt.TTF";

// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// The Font
TTF_Font *font = NULL;

// Init Functions
void init();
bool loadFont(const char *selectedFont, int font_size);
void close_SDL();

#endif