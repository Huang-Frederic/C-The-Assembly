#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const char *FONT_NAME = "american_captain.ttf";
const int FONT_NORMAL = 40;
const int FONT_HOVER = 60;
const char *WINDOW_TITLE = "Slay the Assembly";
const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

SDL_Window *Window;                       // Window created by SDL.
SDL_Renderer *Renderer;                   // The renderer that shows our textures.
SDL_Event WindowEvent;                    // Event capturer from SDL Window.
SDL_Color TextColor = {255, 255, 255, 0}; // White SDL color.
TTF_Font *Font;                           // The font to be loaded from the ttf file.
SDL_Surface *TextSurface;                 // The surface necessary to create the font texture.
SDL_Surface *BackgroundImg = NULL;
SDL_Texture *ImgTexture;                                                // The image we will load and show on the screen                                    // The surface contained by the window
SDL_Texture *TextTexture1, *TextTexture2, *TextTexture3, *TextTexture4; // The font texture prepared for render.
SDL_Rect TextRect1, TextRect2, TextRect3, TextRect4;                    // Text rectangle area with the position for the texture text.
SDL_Point mousePosition;

void createWindow();
void createText(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y);
bool isPollingEvent();
void renderTexts();
void clearMemory();
void createTexts();
void displayMenu();
bool loadMedia();