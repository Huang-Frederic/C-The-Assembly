#include "effect.h"
#include <stdlib.h>

void initEffect(struct Effect *effects, int size) {
    for (int i = 0; i < size; ++i) {
        effects[i].id = -1;
        effects[i].value = -1;
    }
}

int addEffect(struct Effect *effects, int id, int value) {
    for (int i = 0; i < 7; ++i) {
        if (effects[i].id == -1) {
            effects[i].id = id;
            effects[i].value = value;  
            return 1;  
        }
    }
    return 0;
   
}
