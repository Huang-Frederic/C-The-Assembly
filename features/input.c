#include "../headers/include.h"

void input()
{
    // Display the camp screen
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

    char *nickname = malloc(sizeof(char) * 30);
    nickname[0] = '\0';
    char message[100] = "Please Type your Pseudo : ";
    char message_temp[100];
    int nickchar = 0;
    SDL_Event e;
    int clicked = 0;
    renderCombatText(message, 200, gScreenSurface->h / 2, 32);
    SDL_UpdateWindowSurface(gWindow);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_KEYDOWN)
            {
                printf("%s\n", SDL_GetKeyName(e.key.keysym.sym));
                if (strpbrk(SDL_GetKeyName(e.key.keysym.sym), "ABCDEFGHIJKLMNOPQRSTUVWXYZ") != NULL && strlen(SDL_GetKeyName(e.key.keysym.sym)) == 1 && nickchar != 30)
                {
                    sprintf(nickname, "%s%s", nickname, SDL_GetKeyName(e.key.keysym.sym));

                    nickchar++;
                }
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Backspace") == 0)
                {
                    int size = strlen(nickname); // Total size of string
                    nickname[size - 1] = '\0';
                    nickchar--;
                }
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Space") == 0)
                {
                    sprintf(nickname, "%s%s", nickname, " ");
                    nickchar++;
                }
                strcpy(message_temp, message);
                strcat(message_temp, nickname);
                SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                renderCombatText(message_temp, 200, gScreenSurface->h / 2, 32);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
}