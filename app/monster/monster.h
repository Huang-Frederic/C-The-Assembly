#ifndef MONSTER
    #define MONSTER
    struct Monster {
        char name[30];
        int hp;
        int strength;
        int dodge; // 1 = 25% dodge
        int weak; // 1 = attack decreased by 25%
        int defense; // Reduce damage received by x percent
    };

    void createMonster(struct Monster *m, char *name, int hp, int strength, int dodge, int weak, int defense);
    void getMonsterPath(char *name, char *path);
    int checkMonsterPath(char *path);
    int addMonster(struct Monster *m, char *path);
    void printMonster(struct Monster *m);
    int readMonster(char *name);
#endif
