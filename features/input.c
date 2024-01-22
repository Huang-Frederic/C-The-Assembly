#include "../headers/include.h"

void input()
{
    // Display the camp screen
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
    SDL_Surface *Background = load_Pathed_Media("menu/MenuBack", 1);
    display_background(Background);

    char *username = malloc(sizeof(char) * 30);
    strcpy(username, "Cadeon");
    username[7] = '\0';
    char check_char[80] = {"ABCDEFGHIJKLMNOPQRSTUVWXZYabcdefghijklmnopqrstuvwxyz"};
    char message_temp[100] = {"\0"};
    int user_len = 7;
    int textWidth;
    int xCentered;
    SDL_Event e;
    int clicked = 0;

    renderCombatText("Please type your username", 300, gScreenSurface->h / 3 - 100, 48);
    renderMap(load_Pathed_Media("Player", 2), 450, gScreenSurface->h / 3 - 100, 0, 0);
    renderCombatText("Enter to continue ...", gScreenSurface->w - 350, gScreenSurface->h - 60, 28);
    strcpy(message_temp, username);
    textWidth = strlen(message_temp) * 28;
    xCentered = (gScreenSurface->w - textWidth) / 2;
    renderCombatText(message_temp, xCentered, gScreenSurface->h / 2 + 130, 48);

    FadeEffect(0, 0);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {

            switch (e.type)
            {
            case SDL_QUIT:
                if (display_errors_on)
                    fprintf(stderr, "Quitting...");
                close_SDL();

            case SDL_KEYDOWN:
                if (user_len == 0)
                {
                    strcpy(message_temp, "");
                }

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
                else if (strcmp(SDL_GetKeyName(e.key.keysym.sym), "Return") == 0)
                {
                    if (strpbrk(username, check_char) != NULL)
                    {
                        // FadeEffect(0, 1);
                        clicked = 1;
                        display_difficulties(username);
                    }
                }

                strcpy(message_temp, username);
                SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0, 0, 0));
                display_background(Background);
                renderCombatText("Please type your username", 300, gScreenSurface->h / 3 - 100, 48);
                renderCombatText("Enter to continue ...", gScreenSurface->w - 350, gScreenSurface->h - 60, 28);
                renderMap(load_Pathed_Media("Player", 2), 450, gScreenSurface->h / 3 - 100, 0, 0);
                textWidth = strlen(message_temp) * 28;
                xCentered = (gScreenSurface->w - textWidth) / 2;
                renderCombatText(message_temp, xCentered, gScreenSurface->h / 2 + 130, 48);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
}