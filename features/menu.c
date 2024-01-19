#include "../headers/menu.h"

bool create_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y, int middle)
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
    TextRect->x = middle ? SCREEN_WIDTH / 2 - TextSurface->w / 2 : 350;
    TextRect->y = y + 50 + middle * 50;
    TextRect->w = TextSurface->w;
    TextRect->h = TextSurface->h;

    SDL_FreeSurface(TextSurface);
    TTF_Quit();
    return true;
}

bool is_polling_event_menu()
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

                if (SDL_PointInRect(&mousePosition, &TextRect0))
                    printf("\nContinuer la partie");
                else if (SDL_PointInRect(&mousePosition, &TextRect1))
                    printf("\nNouvelle Partie");
                else if (SDL_PointInRect(&mousePosition, &TextRect2))
                    printf("\nOptions");
                else if (SDL_PointInRect(&mousePosition, &TextRect3))
                    display_score();
                else if (SDL_PointInRect(&mousePosition, &TextRect4))
                    return false;
            }

        case SDL_MOUSEMOTION:
            mousePosition.x = WindowEvent.motion.x;
            mousePosition.y = WindowEvent.motion.y;
            create_menu_texts();

            if (SDL_PointInRect(&mousePosition, &TextRect0))
                create_text("Continuer la partie", FONT_HOVER, &TextTexture0, &TextRect0, -25, 1);
            else if (SDL_PointInRect(&mousePosition, &TextRect1))
                create_text("Nouvelle Partie", FONT_HOVER, &TextTexture1, &TextRect1, save ? TextRect0.y + TextRect0.h : 50, 1);
            else if (SDL_PointInRect(&mousePosition, &TextRect2))
                create_text("Options", FONT_HOVER, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h, 1);
            else if (SDL_PointInRect(&mousePosition, &TextRect3))
                create_text("Scores", FONT_HOVER, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h, 1);
            else if (SDL_PointInRect(&mousePosition, &TextRect4))
                create_text("Quitter", FONT_HOVER, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h, 1);
            break;
        }
    }
    return true;
}

bool is_polling_event_score()
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_score();
            return false;

            // case SDL_MOUSEBUTTONDOWN:
            //     if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            //     {
            //         mousePosition.x = WindowEvent.motion.x;
            //         mousePosition.y = WindowEvent.motion.y;

            //         if (SDL_PointInRect(&mousePosition, &TextRect0))
            //             printf("\nContinuer la partie");
            //         else if (SDL_PointInRect(&mousePosition, &TextRect1))
            //             printf("\nNouvelle Partie");
            //         else if (SDL_PointInRect(&mousePosition, &TextRect2))
            //             printf("\nOptions");
            //         else if (SDL_PointInRect(&mousePosition, &TextRect3))
            //             printf("\nScores");
            //         else if (SDL_PointInRect(&mousePosition, &TextRect4))
            //             return false;
            //     }

            // case SDL_MOUSEMOTION:
            //     mousePosition.x = WindowEvent.motion.x;
            //     mousePosition.y = WindowEvent.motion.y;
            //     create_menu_texts();

            //     if (SDL_PointInRect(&mousePosition, &TextRect0))
            //         create_text("Continuer la partie", FONT_HOVER, &TextTexture0, &TextRect0, -25);
            //     else if (SDL_PointInRect(&mousePosition, &TextRect1))
            //         create_text("Nouvelle Partie", FONT_HOVER, &TextTexture1, &TextRect1, save ? TextRect0.y + TextRect0.h : 50);
            //     else if (SDL_PointInRect(&mousePosition, &TextRect2))
            //         create_text("Options", FONT_HOVER, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h);
            //     else if (SDL_PointInRect(&mousePosition, &TextRect3))
            //         create_text("Scores", FONT_HOVER, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h);
            //     else if (SDL_PointInRect(&mousePosition, &TextRect4))
            //         create_text("Quitter", FONT_HOVER, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h);
            //     break;
        }
    }
    return true;
}

void render()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL); // Paint screen black.
    SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1); // Add text to render queue.
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, TextTexture4, NULL, &TextRect4);
    SDL_RenderCopy(Renderer, TextTexture5, NULL, &TextRect5);
    SDL_RenderPresent(Renderer); // Render everything that's on the queue.
    SDL_Delay(10);               // Delay to prevent CPU overhead as suggested by the user `not2qubit`
}

void check_save()
{
    FILE *fp = fopen("data/save.txt", "r");
    if (fp != NULL)
        save = 1;
}

bool create_menu_texts()
{
    if (save)
    {
        if (!create_text("Continuer la partie", FONT_NORMAL, &TextTexture0, &TextRect0, -25, 1))
            return false;
    }
    if (!create_text("Nouvelle Partie", FONT_NORMAL, &TextTexture1, &TextRect1, save ? TextRect0.y + TextRect0.h : 50, 1))
        return false;
    if (!create_text("Options", FONT_NORMAL, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h, 1))
        return false;
    if (!create_text("Scores", FONT_NORMAL, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h, 1))
        return false;
    if (!create_text("Quitter", FONT_NORMAL, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h, 1))
        return false;
    return true;
}

void generate_leaderboard(char (*board)[200])
{
    char username_space[5][70] = {{"\0"}, {"\0"}, {"\0"}, {"\0"}, {"\0"}};

    if (!ranking[0].score)
        select_head();

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 70 - 3 * strlen(ranking[i].username); ++j)
            strcat(username_space[i], " ");
        sprintf(board[i], "%d.    %s%s      %d", i + 1, ranking[i].username, username_space[i], ranking[i].score);
    }
    // for (int i = 0; i < 5; ++i)
    // {
    //     printf("\n%s", board[i]);
    // }
}

bool create_score_texts(char (*board)[200])
{
    if (!create_text("LEADERBOARD", FONT_NORMAL, &TextTexture0, &TextRect0, -20, 1))
        return false;
    if (!create_text(board[0], FONT_NORMAL, &TextTexture1, &TextRect1, TextRect0.y + TextRect0.h + 40, 0))
        return false;
    if (!create_text(board[1], FONT_NORMAL, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h, 0))
        return false;
    if (!create_text(board[2], FONT_NORMAL, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h, 0))
        return false;
    if (!create_text(board[3], FONT_NORMAL, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h, 0))
        return false;
    if (!create_text(board[4], FONT_NORMAL, &TextTexture5, &TextRect5, TextRect4.y + TextRect4.h, 0))
        return false;
    return true;
}

bool display_menu()
{
    check_save();
    if (!load_media())
    {
        printf("\nFailed to load media!");
        return false;
    }

    create_menu_texts();
    while (is_polling_event_menu())
    {
        render();
    }
    return true;
}

bool display_score()
{
    char board[5][200];
    generate_leaderboard(board);
    create_score_texts(board);
    while (is_polling_event_score())
    {
        render();
    }
    return true;
}
void clear_menu()
{
    SDL_FreeSurface(BackgroundImg);
    BackgroundImg = NULL;
    SDL_DestroyTexture(TextTexture0);
    SDL_DestroyTexture(TextTexture1);
    SDL_DestroyTexture(TextTexture2);
    SDL_DestroyTexture(TextTexture3);
    SDL_DestroyTexture(TextTexture4);
    SDL_DestroyTexture(TextTexture5);
    SDL_DestroyTexture(ImgTexture);
}

void clear_score()
{
    TextRect0.h = -1;
    TextRect0.w = -1;
    TextRect0.x = -1;
    TextRect0.y = -1;
    SDL_DestroyTexture(TextTexture0);
    SDL_DestroyTexture(TextTexture1);
    SDL_DestroyTexture(TextTexture2);
    SDL_DestroyTexture(TextTexture3);
    SDL_DestroyTexture(TextTexture4);
    SDL_DestroyTexture(TextTexture5);
}
