#include "../headers/menu.h"

bool load_menu_background(char *img_path)
{
    BackgroundImg = IMG_Load(img_path);
    if (!BackgroundImg)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to load image %s! SDL Error: %s\n", "menu.png", SDL_GetError());
        return false;
    }

    ImgTexture = SDL_CreateTextureFromSurface(Renderer, BackgroundImg);
    if (!ImgTexture)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to create texture from surface.");
        return false;
    }

    return true;
}

bool create_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y, int middle)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
    if (!font)
    {
        if (display_errors_on)
            fprintf(stderr, "\nCouldn't find/init open ttf font.");
        return false;
    }

    TextSurface = TTF_RenderText_Solid(font, Message, TextColor);
    if (TextSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    *TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
    if (*TextTexture == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to create texture from surface!");
        return false;
    }
    TextRect->x = middle ? SCREEN_WIDTH / 2 - TextSurface->w / 2 : 50;
    TextRect->y = y + 50 + middle * 50;
    TextRect->w = TextSurface->w;
    TextRect->h = TextSurface->h;

    SDL_FreeSurface(TextSurface);
    TTF_Quit();
    return true;
}

bool create_difficulty_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int x)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(FONT_NAME, FONT_SIZE);
    if (!font)
    {
        if (display_errors_on)
            fprintf(stderr, "\nCouldn't find/init open ttf font.");
        return false;
    }

    TextSurface = TTF_RenderText_Solid(font, Message, TextColor);
    if (TextSurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to render text! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    *TextTexture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
    if (*TextTexture == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to create texture from surface!");
        return false;
    }
    TextRect->x = x - TextSurface->w / 2;
    TextRect->y = SCREEN_HEIGHT / 2 + 150;
    TextRect->w = TextSurface->w;
    TextRect->h = TextSurface->h;

    SDL_FreeSurface(TextSurface);
    TTF_Quit();
    return true;
}

bool create_difficulty(char *difficulty, SDL_Surface *DifficultySurface, SDL_Texture **DifficultyTexture, SDL_Rect *DifficultyRect, int x, int ishover)
{
    char path[40] = {"data/difficulties/\0"};
    strcat(path, difficulty);
    strcat(path, ".png");
    DifficultySurface = IMG_Load(path);
    if (DifficultySurface == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to render img! Error: %s\n", SDL_GetError());
        return false;
    }
    *DifficultyTexture = SDL_CreateTextureFromSurface(Renderer, DifficultySurface);
    if (*DifficultyTexture == NULL)
    {
        if (display_errors_on)
            fprintf(stderr, "\nUnable to create texture from surface!");
        return false;
    }
    DifficultyRect->x = x;
    DifficultyRect->y = ishover ? SCREEN_HEIGHT / 2 - 150 : (strcmp(difficulty, "validate") == 0) ? SCREEN_HEIGHT - 200 : SCREEN_HEIGHT/ 2 - 100;
    DifficultyRect->w = DifficultySurface->w;
    DifficultyRect->h = DifficultySurface->h;
    SDL_FreeSurface(DifficultySurface);
    return true;
}

bool is_polling_event_menu()
{
    int close_menu = 0;
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_menu();
            close_SDL();
            return false;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;

                if (SDL_PointInRect(&mousePosition, &TextRect0))
                {
                    // Continue
                    menu_event = 1;
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &TextRect1))
                {
                    // NewGame
                    menu_event = 2;
                    return false;
                }
                else if (SDL_PointInRect(&mousePosition, &TextRect2))
                {
                    // History
                    menu_event = 3;
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &TextRect3))
                {
                    menu_event = 4;
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &TextRect4))
                {
                    clear_menu();
                    close_SDL();
                    close_menu = 1;
                }
            }

        case SDL_MOUSEMOTION:
            if (!close_menu)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;
                create_menu_texts();

                if (SDL_PointInRect(&mousePosition, &TextRect0))
                    create_text("Continue the game", FONT_HOVER, &TextTexture0, &TextRect0, -25, 1);
                else if (SDL_PointInRect(&mousePosition, &TextRect1))
                    create_text("New game", FONT_HOVER, &TextTexture1, &TextRect1, save ? TextRect0.y + TextRect0.h : 50, 1);
                else if (SDL_PointInRect(&mousePosition, &TextRect2))
                    create_text("History", FONT_HOVER, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h, 1);
                else if (SDL_PointInRect(&mousePosition, &TextRect3))
                    create_text("Leaderboard", FONT_HOVER, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h, 1);
                else if (SDL_PointInRect(&mousePosition, &TextRect4))
                    create_text("Leave", FONT_HOVER, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h, 1);
                break;
            }
        }
    }
    return true;
}

bool is_polling_event_new_game()
{
    //////////////////////
    SurfaceTheRender(0, 1);
    SDL_Event e;
    int clicked = 0;
    while (SDL_PollEvent(&e) != 0 || clicked == 0)
    {
        if (e.type == SDL_QUIT)
        {
            if (display_errors_on)
                fprintf(stderr, "\n\nQuiting ...\n\n");
            close_SDL();
            return false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            clicked = 1;
        }
    }
    SurfaceTheRender(1, 1);
    //////////////////////
    switch (WindowEvent.type)
    {
    case SDL_QUIT:
        clear_menu();
        close_SDL();
        return false;

    case SDL_MOUSEBUTTONDOWN:
        if (WindowEvent.button.button == SDL_BUTTON_LEFT)
        {
            switch (tuto)
            {
            case 0:
                ++tuto;
                display_tuto(1);
                return false;
            case 1:
                ++tuto;
                display_tuto(2);
                return false;
            case 2:
                tuto = 0;
                input();
                return false;
            }
        }
    }
    return true;
}

bool is_polling_event_difficulties(char *username)
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_menu();
            close_SDL();
            return false;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;

                if (SDL_PointInRect(&mousePosition, &EasyRect))
                {
                    create_save(-1, username);
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &NormalRect))
                {
                    create_save(0, username);
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &HardRect))
                {
                    create_save(1, username);
                    return false;
                }

                else if (SDL_PointInRect(&mousePosition, &PersoRect))
                {
                    display_custom_difficulty(username);
                    return false;
                }
            }
        case SDL_MOUSEMOTION:
            mousePosition.x = WindowEvent.motion.x;
            mousePosition.y = WindowEvent.motion.y;
            create_difficulties();

            if (SDL_PointInRect(&mousePosition, &EasyRect))
                create_difficulty("Easy", EasyImg, &EasyTexture, &EasyRect, 100, 1);
            else if (SDL_PointInRect(&mousePosition, &NormalRect))
                create_difficulty("Normal", NormalImg, &NormalTexture, &NormalRect, 400, 1);
            else if (SDL_PointInRect(&mousePosition, &HardRect))
                create_difficulty("Hard", HardImg, &HardTexture, &HardRect, 700, 1);
            else if (SDL_PointInRect(&mousePosition, &PersoRect))
                create_difficulty("Perso", PersoImg, &PersoTexture, &PersoRect, 1000, 1);
            break;
        }
    }

    return true;
}


bool is_polling_event_custom_difficulty(char *username)
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_menu();
            close_SDL();
            return false;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;

                if (SDL_PointInRect(&mousePosition, &RemoveRect))
                {
                    custom_difficulty -= (custom_difficulty > -50 ? (custom_difficulty == 10 ? 20 : 10) : 0); 
                    if (display_errors_on) 
                        fprintf(stderr, "Custom: %d\n", custom_difficulty);
                    create_custom_difficulty();
                }

                else if (SDL_PointInRect(&mousePosition, &AddRect))
                {
                    custom_difficulty += (custom_difficulty < 200 ? (custom_difficulty == -10 ? 20 : 10) : 0);  
                    if (display_errors_on)
                        fprintf(stderr, "Custom: %d\n", custom_difficulty);
                    create_custom_difficulty();
                }

                else if (SDL_PointInRect(&mousePosition, &TextRect0))
                {
                    display_difficulties(username);
                    return false;
                }

                
                else if (SDL_PointInRect(&mousePosition, &ValidRect))
                {
                    create_save(custom_difficulty, username);
                    start_game();
                    return false;
                }
            }
        }
    }

    return true;
}


void render()
{

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);
    if (save)
    {
        SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    }
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1);
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, TextTexture4, NULL, &TextRect4);
    SDL_RenderCopy(Renderer, TextTexture5, NULL, &TextRect5);
    SDL_RenderPresent(Renderer);
    SDL_Delay(10);
}

void render_new_game()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);
    SDL_RenderPresent(Renderer);
    SDL_Delay(10);
}

void render_difficulties()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);
    SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1);
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, EasyTexture, NULL, &EasyRect);
    SDL_RenderCopy(Renderer, NormalTexture, NULL, &NormalRect);
    SDL_RenderCopy(Renderer, HardTexture, NULL, &HardRect);
    SDL_RenderCopy(Renderer, PersoTexture, NULL, &PersoRect);
    SDL_RenderPresent(Renderer);
    SDL_Delay(10);
}

void render_custom_difficulty()
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);
    SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1);
    SDL_RenderCopy(Renderer, RemoveTexture, NULL, &RemoveRect);
    SDL_RenderCopy(Renderer, AddTexture, NULL, &AddRect);
    SDL_RenderCopy(Renderer, ValidTexture, NULL, &ValidRect);

    SDL_RenderPresent(Renderer);
    SDL_Delay(10);
}

void check_save()
{
    FILE *fp = fopen("data/save.txt", "r");
    if (fp != NULL)
    {
        fclose(fp);
        save = 1;
    }
    else
    {
        save = 0;
    }
}

bool create_menu_texts()
{
    if (save)
    {
        if (!create_text("Continue the game", FONT_NORMAL, &TextTexture0, &TextRect0, -25, 1))
            return false;
    }
    if (!create_text("New game", FONT_NORMAL, &TextTexture1, &TextRect1, save ? TextRect0.y + TextRect0.h : 50, 1))
        return false;
    if (!create_text("History", FONT_NORMAL, &TextTexture2, &TextRect2, TextRect1.y + TextRect1.h, 1))
        return false;
    if (!create_text("Leaderboard", FONT_NORMAL, &TextTexture3, &TextRect3, TextRect2.y + TextRect2.h, 1))
        return false;
    if (!create_text("Leave", FONT_NORMAL, &TextTexture4, &TextRect4, TextRect3.y + TextRect3.h, 1))
        return false;
    return true;
}

bool create_difficulties()
{
    if (!create_difficulty("Easy", EasyImg, &EasyTexture, &EasyRect, 100, 0))
        return false;
    if (!create_difficulty("Normal", NormalImg, &NormalTexture, &NormalRect, 400, 0))
        return false;
    if (!create_difficulty("Hard", HardImg, &HardTexture, &HardRect, 700, 0))
        return false;
    if (!create_difficulty("Perso", PersoImg, &PersoTexture, &PersoRect, 1000, 0))
        return false;
    if (!create_difficulty_text("Easy", 50, &TextTexture0, &TextRect0, EasyRect.x + EasyRect.w / 2))
        return false;
    if (!create_difficulty_text("Normal", 50, &TextTexture1, &TextRect1, NormalRect.x + NormalRect.w / 2))
        return false;
    if (!create_difficulty_text("Hard", 50, &TextTexture2, &TextRect2, HardRect.x + HardRect.w / 2))
        return false;
    if (!create_difficulty_text("Custom", 50, &TextTexture3, &TextRect3, PersoRect.x + PersoRect.w / 2))
        return false;
    return true;
}

bool create_custom_difficulty() {
    char pourcent[6];
    snprintf(pourcent, 5, "%d", custom_difficulty);
    strcat(pourcent, "%");
    
    if (!create_text("<- Retour", FONT_NORMAL, &TextTexture0, &TextRect0, 0, 0))
        return false;
    if (!create_text(pourcent, FONT_HOVER, &TextTexture1, &TextRect1, SCREEN_HEIGHT / 2 - 100, 1))
        return false;
    if (!create_difficulty("remove_button", RemoveImg, &RemoveTexture, &RemoveRect, SCREEN_WIDTH / 2 - 400, 0))
        return false;
    if (!create_difficulty("add_button", AddImg, &AddTexture, &AddRect, SCREEN_WIDTH / 2 + 200, 0))
        return false;
    if (!create_difficulty("validate", ValidImg, &ValidTexture, &ValidRect, SCREEN_WIDTH - 250, 0))
        return false;
}

void display_menu()
{
    check_save();
    if (!load_menu_background("data/menu/menu.png"))
    {
        if (display_errors_on)
            fprintf(stderr, "\nFailed to load media!");
        close_SDL();
    }

    create_menu_texts();

    SurfaceTheRender(0, 0);

    while (is_polling_event_menu())
    {
        render();
    }
    SurfaceTheRender(1, 0);
    switch (menu_event)
    {
    case 1:
        start_game();
        break;
    case 2:
        display_new_game();
        break;
    case 3:
        display_history();
        break;
    case 4:
        display_score();
        break;
    }
}

void display_score()
{
    // Get the Leaderboard position

    select_head();
    int clicked = 0;
    char ScoreInChar[15];
    SDL_Event e;

    SDL_Surface *Background = load_Pathed_Media("menu/MenuBack", 1);
    display_background(Background);

    renderCombatText("LeaderBoard", 500, 120, 48);
    for (int i = 0; i < 5; i++)
    {
        snprintf(ScoreInChar, sizeof(ScoreInChar), "%d", ranking[i].score);
        renderCombatText(ScoreInChar, 820, 240 + (i * 70), 32);
        renderCombatText(ranking[i].username, 420, 240 + (i * 70), 32);
    }
    SDL_UpdateWindowSurface(gWindow);

    FadeEffect(0, 0);
    SDL_Delay(600);

    renderMap(load_Pathed_Media("others/arrow", 0.7), gScreenSurface->w - 170, gScreenSurface->h - 150, 0, 0);
    SDL_UpdateWindowSurface(gWindow);

    while (clicked == 0)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                if (display_errors_on)
                    fprintf(stderr, "\n\nQuiting ...\n\n");
                close_SDL();
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                clicked = 1;
            }
        }
    }

    FadeEffect(0, 1);
}

void display_new_game()
{
    if (!load_menu_background("data/menu/history.png"))
    {
        if (display_errors_on)
            fprintf(stderr, "\nFailed to load media!");
        close_SDL();
    }

    while (is_polling_event_new_game())
    {
        render_new_game();
    }
}

void display_history()
{
    SDL_Event e;
    int clicked = 0;

    SDL_Surface *HistoryImg = load_Pathed_Media("menu/history", 1);
    display_background(HistoryImg);
    SDL_UpdateWindowSurface(gWindow);

    FadeEffect(0, 0);
    SDL_Delay(600);

    renderMap(load_Pathed_Media("others/arrow", 0.7), gScreenSurface->w - 170, gScreenSurface->h - 150, 0, 0);
    SDL_UpdateWindowSurface(gWindow);

    while (SDL_PollEvent(&e) != 0 || clicked == 0)
    {
        if (e.type == SDL_QUIT)
        {
            if (display_errors_on)
                fprintf(stderr, "\n\nQuiting ...\n\n");
            close_SDL();
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            clicked = 1;
        }
    }
    FadeEffect(0, 1);
}

void SurfaceTheRender(int FadeType, int Clear_Render)
{
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL); // Paint screen black.
    if (save)
    {
        SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    }
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1); // Add text to render queue.
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, TextTexture4, NULL, &TextRect4);
    SDL_RenderCopy(Renderer, TextTexture5, NULL, &TextRect5);

    if (Clear_Render == 1)
    {
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
        SDL_RenderClear(Renderer);
        SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL); // Paint screen black.
    }

    int width, height;
    SDL_GetRendererOutputSize(Renderer, &width, &height);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_Surface *finalSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_BlitSurface(finalSurface, NULL, gScreenSurface, NULL);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(finalSurface);

    FadeEffect(0, FadeType);
}

void display_tuto(int num)
{
    switch (num)
    {
    case 1:
        if (!load_menu_background("data/menu/tutoriel_1.png"))
        {
            if (display_errors_on)
                fprintf(stderr, "\nFailed to load media!");
            close_SDL();
        }
        break;
    case 2:
        if (!load_menu_background("data/menu/tutoriel_2.png"))
        {
            if (display_errors_on)
                fprintf(stderr, "\nFailed to load media!");
            close_SDL();
        }
        break;
    }
    while (is_polling_event_new_game())
    {

        render_new_game();
    }
}

void display_difficulties(char *username)
{

    if (!load_menu_background("data/menu/MenuBack.png"))
    {
        if (display_errors_on)
            fprintf(stderr, "\nFailed to load media!");
        close_SDL();
    }
    create_difficulties();

    // FadeDifficulty(0);

    while (is_polling_event_difficulties(username))
    {
        render_difficulties();
    }

    // FadeDifficulty(1);

    start_game();
}

void FadeDifficulty(int inout)
{
    int width, height;

    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL);
    SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1); // Add text to render queue.
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, TextTexture3, NULL, &TextRect3);
    SDL_RenderCopy(Renderer, EasyTexture, NULL, &EasyRect);
    SDL_RenderCopy(Renderer, NormalTexture, NULL, &NormalRect);
    SDL_RenderCopy(Renderer, HardTexture, NULL, &HardRect);
    SDL_RenderCopy(Renderer, PersoTexture, NULL, &PersoRect);

    SDL_GetRendererOutputSize(Renderer, &width, &height);

    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    SDL_Surface *finalSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_BlitSurface(finalSurface, NULL, gScreenSurface, NULL);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(finalSurface);

    FadeEffect(0, inout);
}


void display_custom_difficulty(char *username)
{

    if (!load_menu_background("data/menu/MenuBack.png"))
    {
        if (display_errors_on)
            fprintf(stderr, "\nFailed to load media!");
        close_SDL();
    }
    create_custom_difficulty();


    while (is_polling_event_custom_difficulty(username))
    {
        render_custom_difficulty();
    }
}



void start_game()
{
    RETURN_TO_MENU = 0;
    save_to_player();
    while (RETURN_TO_MENU == 0)
    {
        char *selected_map = map();
        if (display_errors_on)
            fprintf(stderr, "Selected: %s\n\n", selected_map);
        if (RETURN_TO_MENU == 0)
        {
            event(selected_map);
        }
    }
}

bool init_save(struct Save *save)
{
    save->deck_size = 10;
    save->day = 1;
    save->hp = 100;
    save->max_hp = 100;
    save->max_energy = 5;
    save->score = 0;
}

bool create_save(int difficulty, char *username)
{
    struct Save save;
    init_save(&save);
    save.difficulty = difficulty;
    strcpy(save.player_name, username);
    if (!create_save_file(save))
    {
        if (display_errors_on)
            fprintf(stderr, "\nError while creating the save\n");
        return false;
    }
    return true;
}

bool create_save_file(const struct Save save)
{
    char init_deck[10][30] = {"__Player__Pointer-Pistol-Shot", "__Player__Pointer-Pistol-Shot", "__Player__C-Bang-Pointer-Pop",
                              "__Player__C-Bang-Pointer-Pop", "__Player__C-Bang-Pointer-Pop", "__Player__C-Guard", "__Player__Celerity-Compiler",
                              "__Player__Code-Breach", "__Player__Lethal-Remedy-Drop", "__Player__HemoHustle-Nudge"};
    FILE *save_file = fopen("data/save.txt", "w");
    if (save_file == NULL)
        return false;
    fprintf(save_file, "%d\n%d\n%s\n%d\n%d\n%d\n%d\n%d", save.deck_size, save.day, save.player_name, save.difficulty, save.hp, save.max_hp, save.max_energy, save.score);
    for (int i = 0; i < 10; ++i)
    {
        fprintf(save_file, "\n%s", init_deck[i]);
    }
    fclose(save_file);
    return true;
}

void clear_menu()
{
    SDL_FreeSurface(BackgroundImg);
    SDL_FreeSurface(EasyImg);
    SDL_FreeSurface(NormalImg);
    SDL_FreeSurface(HardImg);
    BackgroundImg = NULL;
    SDL_DestroyTexture(TextTexture0);
    SDL_DestroyTexture(TextTexture1);
    SDL_DestroyTexture(TextTexture2);
    SDL_DestroyTexture(TextTexture3);
    SDL_DestroyTexture(TextTexture4);
    SDL_DestroyTexture(TextTexture5);
    SDL_DestroyTexture(ImgTexture);
    SDL_DestroyTexture(EasyTexture);
    SDL_DestroyTexture(NormalTexture);
    SDL_DestroyTexture(HardTexture);
}
