#include <stdio.h>
#include <string.h>
#include "monster.h"

void createMonster(struct Monster *m, char *name, int hp, int strength, int dodge, int weak, int defense) {
    strcpy(m->name, name);
    m->hp = hp;
    m->strength = strength;
    m->dodge = dodge;
    m->weak = weak;
    m->defense = defense;
}


void getMonsterPath(char *name, char *path) {
    strcpy(path, "..\\data\\monster\\");
    strcat(path, name);
    strcat(path, ".bin");
}


int checkMonsterPath(char *path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;
    return 1;
}


int addMonster(struct Monster *m, char *path) {
    if (checkMonsterPath(path)) {
        printf("\nFile with this name already exists.");
        return 0;
    }
        
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return 0;
    } 
    fwrite(m, sizeof(struct Monster), 1, fp);
    fclose(fp);
    return 1;
} 

void printMonster(struct Monster *m) {
    printf("\n Name: %s | HP: %d | Strength: %d | Dodge: %d | Weak: %d | Defense: %d",
    m->name, m->hp, m->strength, m->dodge, m->weak, m->defense);
}


int readMonster(char *name) {
    FILE *fp;
    struct Monster m;
    char path[100];
   
    getMonsterPath(name, path);
    if (!checkMonsterPath(path)) {
        printf("\nFile doesn't exist.");
        return 0;
    }

    fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;
    
    fread(&m, sizeof(struct Monster), 1, fp);
    printMonster(&m);
    fclose(fp);
    return 1;
}
