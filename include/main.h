#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const char *WINDOW_TITLE = "Slay the Assembly";
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

SDL_Window *Window; // Window created by SDL.
SDL_Event WindowEvent;

bool createWindow();
bool loadMedia();