#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sqlite3.h>
#include <string.h>

const char *WINDOW_TITLE = "Slay the Assembly";
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

SDL_Window *Window;
SDL_Event WindowEvent;
SDL_Renderer *Renderer;

bool create_window();
bool load_media();
void clear_window();
