#include "card/card.c"
#include "monster/monster.c"

void testCard() {
    // struct Effect effects[7] = {{0}};
    
    // struct Card c;

    // initEffect(effects, 7);
   
   
    // addEffect(effects, 1, 6);
    // printf("%d", effects[0].id);
    // createCard(&c, "rayon", 1, "description", effects);
    // char path[100];
    // getCardPath(c.name, path);
    // int success = addCard(&c, path);
    // if (!success)
    //     return 0;
    // // return 1;
    // printCard(&c);
    // // char *name = "test";
    // char name[20] = "rayon";
    // readCard(name);
}

void testMonster() {
    struct Monster m;
    createMonster(&m, "RAX", 70, 0, 0, 0, 0);
    char path[100];
    getMonsterPath(m.name, path);
    int success = addMonster(&m, path);
    if (!success)
        return 0;
    // return 1;
    printMonster(&m);
    char *name = "test";
    char name[20] = "RAX";
    readMonster(name);
}

int main(int argc, char * argv[]) {
  

 


}