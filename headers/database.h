struct Ranking
{
    int id;
    char username[30];
    int score;
};

int counter = 0;
int exist = 0;
struct Ranking ranking[5];

int callback(void *NotUsed, int argc, char **argv, char **azColName);
int check_callback(void *NotUsed, int argc, char **argv, char **azColName);
bool create();
bool insert(int score, char *username);
bool select_head();
bool select_by_username(char *username);
bool check_username(char *username);
bool update(int score, char *username);
bool delete(char *username);
bool delete_all();
void display_ranking_head();