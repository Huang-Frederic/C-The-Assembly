const char *FONT_NAME = "data/font/upheavtt.ttf";
const int FONT_NORMAL = 40;
const int FONT_HOVER = 60;

SDL_Color TextColor = {255, 255, 255, 0}; 
TTF_Font *Font;                           
SDL_Surface *TextSurface;                 
SDL_Surface *BackgroundImg = NULL;
SDL_Surface *EasyImg, *NormalImg, *HardImg, *PersoImg;
SDL_Texture *ImgTexture;
SDL_Texture *EasyTexture, *NormalTexture, *HardTexture, *PersoTexture;                                               
SDL_Texture *TextTexture0, *TextTexture1, *TextTexture2, *TextTexture3, *TextTexture4, *TextTexture5; 
SDL_Rect EasyRect, NormalRect, HardRect, PersoRect;
SDL_Rect TextRect0, TextRect1, TextRect2, TextRect3, TextRect4, TextRect5; 
SDL_Point mousePosition;

bool create_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y, int middle);
bool create_menu_texts();
bool is_polling_event_menu();
bool load_menu_background(char *img_path);
void display_menu();
void check_save();
void render();
void clear_menu();
void display_history();
void SurfaceTheRender(int FadeType, int Clear_Render);

bool create_difficulty_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int x);
bool create_difficulty(char *difficulty, SDL_Surface *DifficultySurface, SDL_Texture **DifficultyTexture, SDL_Rect *DifficultyRect, int x, int ishover);
bool create_difficulties();
void FadeDifficulty(int inout);
void display_difficulties(char *username);
bool is_polling_event_difficulties(char *username);
void render_difficulties();

void display_new_game();
void render_new_game();
bool is_polling_event_new_game();

void display_score();

void display_tuto(int num);

void display_history();

void input();

bool init_save(struct Save *save);
bool create_save(int difficulty, char *username);
bool create_save_file(const struct Save save);

void start_game();

int save = 0;
int menu_event = 0;
int tuto = 0;

extern struct Ranking ranking[5];