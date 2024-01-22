
#ifndef DEFAULT
#define DEFAULT

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const char *selectedFont = "data/font/upheavtt.TTF";

// Windows Title
const char *WINDOW_TITLE = "C the Assembly";
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
// The Font
TTF_Font *font = NULL;

SDL_Event WindowEvent;
SDL_Renderer *Renderer;

int RETURN_TO_MENU = 0;

// Init Functions
void init();
bool loadFont(const char *selectedFont, int font_size);
void close_SDL();
bool create_window();
bool load_media();
void clear_window();
void get_auto_save();
void get_display_errors();

int auto_save_on = 1;
int display_errors_on = 1;

#endif