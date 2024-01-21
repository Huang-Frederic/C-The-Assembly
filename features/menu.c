#include "../headers/menu.h"

bool load_menu_background(char *img_path)
{
    BackgroundImg = IMG_Load(img_path);
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

bool create_difficulty_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int x) {
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
    TextRect->x = x - TextSurface->w/2;
    TextRect->y = SCREEN_HEIGHT/2 + 150;
    TextRect->w = TextSurface->w;
    TextRect->h = TextSurface->h;

    SDL_FreeSurface(TextSurface);
    TTF_Quit();
    return true;
    
}

bool create_difficulty(char *difficulty, SDL_Surface *DifficultySurface, SDL_Texture **DifficultyTexture, SDL_Rect *DifficultyRect, int x) {
    char path[40] = {"data/difficulties/\0"};
    strcat(path, difficulty);
    strcat(path, ".png");
    DifficultySurface = IMG_Load(path); 
    if (DifficultySurface == NULL)
    {
        printf("\nUnable to render img! Error: %s\n", SDL_GetError());
        return false;
    }
    *DifficultyTexture = SDL_CreateTextureFromSurface(Renderer, DifficultySurface); 
     if (*DifficultyTexture == NULL)
    {
        printf("\nUnable to create texture from surface!");
        return false;
    }
    DifficultyRect->x = x;
    DifficultyRect->y = SCREEN_HEIGHT / 2 - 100;
    DifficultyRect->w = DifficultySurface->w;
    DifficultyRect->h = DifficultySurface->h;
    SDL_FreeSurface(DifficultySurface);
    return true;
}

int is_polling_event_menu()
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_menu();
            close_SDL();
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;

                if (SDL_PointInRect(&mousePosition, &TextRect0))
                    printf("\nContinue the game");
                else if (SDL_PointInRect(&mousePosition, &TextRect1)) {
                    menu_event = 2;
                    return 1;
                }
                else if (SDL_PointInRect(&mousePosition, &TextRect2)) {
                    printf("\nHistory");
                    menu_event = 3;
                }
                    
                else if (SDL_PointInRect(&mousePosition, &TextRect3)) {
                    // display_score();
                    menu_event = 4;
                }
                    
                else if (SDL_PointInRect(&mousePosition, &TextRect4)) {
                    clear_menu();
                    close_SDL();
                }
                    
            }

        case SDL_MOUSEMOTION:
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
    return 0;
}

bool is_polling_event_score()
{
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_score();
            // close_SDL();
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
    return 0;
}


bool is_polling_event_new_game() {
     while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_score();
            close_SDL();
            return false;

        case SDL_MOUSEBUTTONDOWN:
            if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                switch(tuto) {
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
    
    }   
    return true;      
}

bool is_polling_event_difficulties(char *username) {
    while (SDL_PollEvent(&WindowEvent))
    {
        switch (WindowEvent.type)
        {
        case SDL_QUIT:
            clear_score();
            close_SDL();
            return false;

        case SDL_MOUSEBUTTONDOWN:
        if (WindowEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousePosition.x = WindowEvent.motion.x;
                mousePosition.y = WindowEvent.motion.y;

                if (SDL_PointInRect(&mousePosition, &EasyRect)) {
                    create_save(-1, username);
                    return false;
                }
                    
                else if (SDL_PointInRect(&mousePosition, &NormalRect)) {
                    create_save(0, username);
                    return false;
                }
                  
                else if (SDL_PointInRect(&mousePosition, &HardRect)) {
                    create_save(1, username);
                    return false;
                }
                    
            }
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

void render_new_game() {
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL); // Paint screen black.
    SDL_RenderPresent(Renderer); // Render everything that's on the queue.
    SDL_Delay(10);               // Delay to prevent CPU overhead as suggested by the user `not2qubit`
}

void render_difficulties() {
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(Renderer);
    // SDL_RenderCopy(Renderer, ImgTexture, NULL, NULL); 
    SDL_RenderCopy(Renderer, TextTexture0, NULL, &TextRect0);
    SDL_RenderCopy(Renderer, TextTexture1, NULL, &TextRect1); // Add text to render queue.
    SDL_RenderCopy(Renderer, TextTexture2, NULL, &TextRect2);
    SDL_RenderCopy(Renderer, EasyTexture, NULL, &EasyRect);
    SDL_RenderCopy(Renderer, NormalTexture, NULL, &NormalRect);
    SDL_RenderCopy(Renderer, HardTexture, NULL, &HardRect);
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
    if (!create_difficulty("Easy", EasyImg, &EasyTexture, &EasyRect, 250))
        return false;
    if (!create_difficulty("Normal", NormalImg, &NormalTexture, &NormalRect, 550))
        return false;
    if (!create_difficulty("Hard", HardImg, &HardTexture, &HardRect, 850))
        return false;
    if (!create_difficulty_text("Easy", 50, &TextTexture0, &TextRect0, EasyRect.x + EasyRect.w / 2))
        return false;
    if (!create_difficulty_text("Normal", 50, &TextTexture1, &TextRect1, NormalRect.x + NormalRect.w / 2))
        return false;
    if (!create_difficulty_text("Hard", 50, &TextTexture2, &TextRect2, HardRect.x + HardRect.w / 2))
        return false;
    return true;
} 


void generate_leaderboard(char (*board)[200])
{
    char username_space[5][70] = {{"\0"}, {"\0"}, {"\0"}, {"\0"}, {"\0"}};

    select_head();

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 70 - 3 * strlen(ranking[i].username); ++j)
            strcat(username_space[i], " ");
        sprintf(board[i], "%d.    %s%s      %d", i + 1, ranking[i].username, username_space[i], ranking[i].score);
    }
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

void display_menu()
{
    check_save();
    if (!load_menu_background("data/menu/menu.jpg"))
    {
        printf("\nFailed to load media!");
        close_SDL();
    }

    create_menu_texts();
    while (is_polling_event_menu() == 0)
    {
        render();
    }
    switch (menu_event)
    {
    case 2:
        display_new_game();
        break;
    }
}

void display_score()
{
    char board[5][200];
    generate_leaderboard(board);
    create_score_texts(board);
    while (is_polling_event_score())
    {
        render();
    }
}

void display_new_game() {
    if (!load_menu_background("data/menu/history.jpg"))
    {
        printf("\nFailed to load media!");
        close_SDL();
    }
    while (is_polling_event_new_game())
    {
        render_new_game();
    }
}

void display_tuto(int num) {
    switch (num) {
        case 1:
            if (!load_menu_background("data/menu/tutoriel_1.jpg"))
            {
                printf("\nFailed to load media!");
                close_SDL();
            }
            break;
        case 2:
        if (!load_menu_background("data/menu/tutoriel_2.jpg"))
            {
                printf("\nFailed to load media!");
                close_SDL();
            }
        break;
    }
    while (is_polling_event_new_game())
    {
        render_new_game();
    }
}

void display_difficulties(char *username) {;
    if (!load_menu_background("data/menu/difficulties.jpg"))
        {
            printf("\nFailed to load media!");
            close_SDL();
        }
    create_difficulties();
    while (is_polling_event_difficulties(username))
    {
        render_difficulties();
    }
}

bool init_save(struct Save *save) {
   save->deck_size = 10;
   save->day = 1;
   save->hp = 100;
   save->max_hp = 100;
   save->max_energy = 5;
   save->score = 0;
}

bool create_save(int difficulty, char *username) {
    struct Save save;
    init_save(&save);
    save.difficulty = difficulty;
    strcpy(save.player_name, username);
    if (!create_save_file(save)) {
        printf("\nError while creating the save\n");
        return false;
    }
    return true;

}

bool create_save_file (const struct Save save) {
    char init_deck[10][30] = {"__Player__Pointer-Pistol-Shot", "__Player__Pointer-Pistol-Shot", "__Player__C-Bang-Pointer-Pop",
    "__Player__C-Bang-Pointer-Pop", "__Player__C-Bang-Pointer-Pop", "__Player__C-Guard", "__Player__Celerity-Compiler", 
    "__Player__Code-Breach", "__Player__Lethal-Remedy-Drop", "__Player__HemoHustle-Nudge"};
    FILE *save_file = fopen("data/save.txt", "w");
    if (save_file == NULL)
        return false;
    fprintf(save_file, "%d\n%d\n%s\n%d\n%d\n%d\n%d\n%d", save.deck_size, save.day, save.player_name, save.difficulty, save.hp, save.max_hp, save.max_energy, save.score);
    for (int i = 0; i < 10; ++i) {
        fprintf(save_file, "\n%s", init_deck[i]);
    }
    fclose(save_file);
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
    // TextRect0.h = -1;
    // TextRect0.w = -1;
    // TextRect0.x = -1;
    // TextRect0.y = -1;
    SDL_DestroyTexture(TextTexture0);
    SDL_DestroyTexture(TextTexture1);
    SDL_DestroyTexture(TextTexture2);
    SDL_DestroyTexture(TextTexture3);
    SDL_DestroyTexture(TextTexture4);
    SDL_DestroyTexture(TextTexture5);
}
