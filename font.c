#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
                       TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{

    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect->x = (WINDOW_WIDTH / 2) - surface->w / 2;
    rect->y = y + 50;
    rect->w = surface->w;
    rect->h = surface->h;

    printf("%d\n", surface->h);
}

int main(int argc, char **argv)
{
    SDL_Event event;
    SDL_Rect rect1, rect2, rect3, rect4;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2, *texture3, *texture4;
    SDL_Window *window;
    char *fontPath = "american_captain.ttf";

    int quit;

    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(fontPath, 50);

    if (font == NULL)
    {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    get_text_and_rect(renderer, 0, 150, "1. Nouvelle Partie", font, &texture1, &rect1);
    get_text_and_rect(renderer, 0, rect1.y + rect1.h, "2. Options", font, &texture2, &rect2);
    get_text_and_rect(renderer, 0, rect2.y + rect2.h, "3. Score", font, &texture3, &rect3);
    get_text_and_rect(renderer, 0, rect3.y + rect3.h, "4. Quitter", font, &texture4, &rect4);

    quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event) == 1)
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        /* Use TTF textures. */
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderCopy(renderer, texture3, NULL, &rect3);
        SDL_RenderCopy(renderer, texture4, NULL, &rect4);

        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}