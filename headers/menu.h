const char *FONT_NAME = "resource/font/upheavtt.ttf";
const int FONT_NORMAL = 40;
const int FONT_HOVER = 60;

// Event capturer from SDL Window.
SDL_Color TextColor = {255, 255, 255, 0}; // White SDL color.
TTF_Font *Font;                           // The font to be loaded from the ttf file.
SDL_Surface *TextSurface;                 // The surface necessary to create the font texture.
SDL_Surface *BackgroundImg = NULL;
SDL_Surface *EasyImg, *NormalImg, *HardImg;
SDL_Texture *ImgTexture;
SDL_Texture *EasyTexture, *NormalTexture, *HardTexture;                                               // The image we will load and show on the screen                                    // The surface contained by the window
SDL_Texture *TextTexture0, *TextTexture1, *TextTexture2, *TextTexture3, *TextTexture4, *TextTexture5; // The font texture prepared for render.
SDL_Rect EasyRect, NormalRect, HardRect;
SDL_Rect TextRect0, TextRect1, TextRect2, TextRect3, TextRect4, TextRect5; // Text rectangle area with the position for the texture text.
SDL_Point mousePosition;

bool create_text(const char *Message, int FONT_SIZE, SDL_Texture **TextTexture, SDL_Rect *TextRect, int y, int middle);
int is_polling_event_menu();
void render();
void clear_menu();
void clear_score();
void display_history();
void SurfaceTheRender(int FadeType, int Clear_Render);
bool create_menu_texts();
bool create_score_texts(char (*board)[200]);
void display_menu();
void check_save();
void generate_leaderboard(char (*board)[200]);
void display_score();
bool load_menu_background(char *img_path);
void display_new_game();
bool load_new_game_background(char *img_path);
void display_tuto(int num);
void input();
void display_difficulties(char *username);
bool init_save(struct Save *save);
bool create_save(int difficulty, char *username);
bool create_save_file(const struct Save save);
bool create_difficulties();
void start_game();

int save = 0;
int menu_event = 0;
int tuto = 0;

extern struct Ranking ranking[5];