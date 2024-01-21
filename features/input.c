#include "../headers/include.h"

void input()
{
    // Display the camp screen
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));

    char *username = malloc(sizeof(char) * 30);
    username[0] = '\0';
    char message[100] = "Please type your username : ";
    char check_char[80] = {"ABCDEFGHIJKLMNOPQRSTUVWXZYabcdefghijklmnopqrstuvwxyz"};
    char message_temp[100];
    int user_len = 0;
    SDL_Event e;
    int clicked = 0;
    renderCombatText(message, 200, gScreenSurface->h / 2, 32);
    SDL_UpdateWindowSurface(gWindow);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            
            switch(e.type)
            {
            case SDL_QUIT:
                close_SDL();

            case SDL_KEYDOWN:
                // printf("%s\n", SDL_GetKeyName(e.key.keysym.sym));
                if (strpbrk(SDL_GetKeyName(e.key.keysym.sym), "ABCDEFGHIJKLMNOPQRSTUVWXYZ") != NULL && strlen(SDL_GetKeyName(e.key.keysym.sym)) == 1 && user_len != 30)
                {
                    sprintf(username, "%s%s", username, SDL_GetKeyName(e.key.keysym.sym));

                    user_len++;
                }
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Backspace") == 0)
                {
                    int size = strlen(username); // Total size of string
                    username[size - 1] = '\0';
                    user_len--;
                }
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Space") == 0)
                {
                    sprintf(username, "%s%s", username, " ");
                    user_len++;
                }
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Return") == 0)
                {
                    if (strpbrk(username, check_char) != NULL) {
                        clicked = 1;
                        display_difficulties(username);
                    }
                }
                strcpy(message_temp, message);
                strcat(message_temp, username);
                SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                renderCombatText(message_temp, 200, gScreenSurface->h / 2, 32);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
}