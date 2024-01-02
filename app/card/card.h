#include <stdio.h>
#include <stdlib.h>
#include "../effect/effect.h"

struct Card {
    char name[30];
    int energyCost;
    char description[150];
    struct Effect effects[7];
};

void createCard(struct Card *newCard, char *name, int energyCost, char *description, struct Effect *arrayEffects);
void getCardPath(char *name, char *path);
int checkCardPath(char *path);
int addCard(struct Card *c, char *path);
int readCard(char *name);
void printCard(struct Card *c);