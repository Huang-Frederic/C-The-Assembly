#include "include/main.h"
#include "feature/database.c"
#include "feature/menu.c"

bool create_window()
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

bool load_media()
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

void clear_window()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    printf("\nClear proccess done.");
}

int main(int argc, char *argv[])
{

    // for (int i = 0; i < 5; ++i)
    //     if (board[i])
    //         printf("\n%s", board[i]);
    int score = 22222222;
    char *username = "Goat";
    insert(score, username);
    create_window();
    display_menu();
    // return 1;
    clear_menu();
    clear_window();
    return 0;
}

// BDD
// int main(int argc, char *argv[])
// {
//     delete_all();
//     int score = 233333;
//     char *username = "Ar";
//     create();

//     insert(score, username);
//     display_ranking_head();
//     return 0;
// }