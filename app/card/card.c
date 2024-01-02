#include <stdio.h>
#include <string.h>
#include "card.h"
#include "../effect/effect.c"

void createCard(struct Card *newCard, char *name, int energyCost, char *description, struct Effect *arrayEffects) {
    strcpy(newCard->name, name);
    newCard->energyCost = energyCost;
    strcpy(newCard->description, description);
    for (int i = 0; i < 7; i++) {
        newCard->effects[i].id = arrayEffects[i].id;
        newCard->effects[i].value = arrayEffects[i].value;
    }
}


void getCardPath(char *name, char *path) { 
    strcpy(path, "..\\data\\card\\"); // Chemin provisoire
    strcat(path, name);
    strcat(path, ".bin");
}


int checkCardPath(char *path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;
    return 1;
}


int addCard(struct Card *c, char *path) {
    if (checkCardPath(path)) {
        printf("\nFile with this name already exists.");
        return 0;
    }
        
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return 0;
    } 
    fwrite(c, sizeof(struct Card), 1, fp);
    fclose(fp);
    return 1;
}


void printCard(struct Card *c) {
    int i = 0;
    printf("\nName: %s | Energy Cost: %d | Description: %s", c->name, c->energyCost, c->description);
        while ((c->effects[i].id != -1) && (i<7)) {
            printf("\nEffect %d | Id: %d | Value: %d", i, c->effects[i].id, c->effects[i].value);
            ++i;
        }       
}


int readCard(char *name) {
    FILE *fp;
    struct Card c;
    char path[100];
   
    getCardPath(name, path);
    if (!checkCardPath(path)) {
        printf("\nFile doesn't exist.");
        return 0;
    }

    fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;
    
    fread(&c, sizeof(struct Card), 1, fp);
    printCard(&c);
    fclose(fp);
    return 1;
}


