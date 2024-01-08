#include "include/main.h"
#include "feature/menu.c"

bool createWindow()
{
    Window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if (!Window)
    {
        printf("\nThere was a problem creating the window.");
        return false;
    }
    Renderer = SDL_CreateRenderer(Window, -1, 0);
    if (!Renderer)
    {
        printf("\nThere was a problem creating the renderer.");
        return false;
    }
    return true;
}

bool loadMedia()
{

    // Load splash image
    BackgroundImg = IMG_Load("resource/img/menu.jpg");
    if (!BackgroundImg)
    {
        printf("\nUnable to load image %s! SDL Error: %s\n", "menu.jpg", SDL_GetError());
        return false;
    }

    ImgTexture = SDL_CreateTextureFromSurface(Renderer, BackgroundImg);
    if (!ImgTexture)
    {
        printf("\nUnable to create texture from surface.");
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    createWindow();
    if (!displayMenu())
        return 1;
    clearMemory();
    return 0;
}