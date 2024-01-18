#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Ranking
{
    int id;
    char username[30];
    int score;
};

int counter = 0;
int exist = 0;
struct Ranking ranking[5];
