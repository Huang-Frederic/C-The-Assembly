#include "include/menu.h"

bool createText(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
    if (!font)
    {
        printf("\nCouldn't find/init open ttf font.");
        return false;
    }

    TextSurface = TTF_RenderText_Solid(font, Message, TextColor);
    if (TextSurface == NULL)
    {
        printf("\nUnable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    *TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
    if (*TextTexture == NULL)
    {
        printf("\nUnable to create texture from surface!");
        return false;
    }
    TextRect->x = WINDOW_WIDTH / 2 - TextSurface->w / 2;
    TextRect->y = y + 100;
    TextRect->w = TextSurface->w;
    TextRect->h = TextSurface->h;
    // After you create the texture you can release the surface memory allocation because we actually render the texture not the surface.
    SDL_FreeSurface(TextSurface);
    TTF_Quit();
    return true;
}

bool isPollingEvent()
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            return false;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;
                if (SDL_PointInRect(&mousePosition, &TextRect1))
                {
                    printf("\nNouvelle Partie");
                }
                else if (SDL_PointInRect(&mousePosition, &TextRect2))
                {
                    printf("\nOptions");
                }
                else if (SDL_PointInRect(&mousePosition, &TextRect3))
                {
                    printf("\nScores");
                }
                else if (SDL_PointInRect(&mousePosition, &TextRect4))
                {
                    return false;
                }
            }

        case SDL_MOUSEMOTION:
            mousePosition.x = WindowEvent.motion.x;
            mousePosition.y = WindowEvent.motion.y;
            createTexts();
            if (SDL_PointInRect(&mousePosition, &TextRect1))
            {
                createText("Nouvelle Partie", FONT_HOVER, &TextTexture1, &TextRect1, 50);
            }
            else if (SDL_PointInRect(&mousePosition, &TextRect2))
            {
                createText("Options", FONT_HOVER, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h);
            }
            else if (SDL_PointInRect(&mousePosition, &TextRect3))
            {
                createText("Scores", FONT_HOVER, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h);
            }
            else if (SDL_PointInRect(&mousePosition, &TextRect4))
            {
                createText("Quitter", FONT_HOVER, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h);
            }
            break;
        }
    }
    return true;
}

void render()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);         // Paint screen black.
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1); // Add text to render queue.
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, TextTexture4, NULL, &TextRect4);
    SDL_RenderPresent(Renderer); // Render everything that's on the queue.
    SDL_Delay(10);               // Delay to prevent CPU overhead as suggested by the user `not2qubit`
}

bool createTexts()
{
    if (!createText("Nouvelle Partie", FONT_NORMAL, &TextTexture1, &TextRect1, 50))
        return false;
    if (!createText("Options", FONT_NORMAL, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h))
        return false;
    if (!createText("Scores", FONT_NORMAL, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h))
        return false;
    if (!createText("Quitter", FONT_NORMAL, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h))
        return false;
    return true;
}

bool displayMenu()
{
    if (!loadMedia())
    {
        printf("\nFailed to load media!");
        return false;
    }

    createTexts();
    while (isPollingEvent())
    {
        render();
    }
    return true;
}

void clearMemory()
{
    SDL_FreeSurface(BackgroundImg);
    BackgroundImg = NULL;

    SDL_DestroyTexture(TextTexture1);
    SDL_DestroyTexture(TextTexture2);
    SDL_DestroyTexture(TextTexture3);
    SDL_DestroyTexture(TextTexture4);
    SDL_DestroyTexture(ImgTexture);

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    printf("\nClear proccess done.");
}
