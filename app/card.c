#include <stdio.h>
#include <string.h>

void getCardPath(char *path, int id) {
    char idString[5];
    sprintf(idString, "%d", id);

    strcat(path, "..\\data\\card\\"); // à changer après les vérifications
    strcat(path, idString);
    strcat(path, ".txt");
    
}


void getCardName (int id) { // à transformer en char*
    FILE *fp;
    char path[100] = {"\0"};
    getCardPath(path, id);
    char line[100];


    fp = fopen(path, "rb");
    if (fp != NULL) {
        while(fgets(line, 100, fp) != NULL)
            printf("%s", line); // -> HANDLE ERROR 
        fclose(fp);
    } else {
        printf("Error"); 
    }
}

void getCardEnergyCost(int id) {

}

void getCardDescription(int id) {

}

void createCard() {

}

void checkSize() { // Pour voir si un string comme name ou description fait bien moins de x caractères.

}

// void inputString(char *string, int size) {
//     fgets(string, size, stdin);
//     if (string[strlen(string) - 1] == '\n')
//         string[strlen(string) - 1] = '\0';
// }





int main() {
    getCardName(1);
    return 0;
}