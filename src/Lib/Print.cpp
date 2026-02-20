#include <cstdio>

extern "C" {

    void printInt(int value) {
        printf("%d", value);
    }

    void printFloat(float value) {
        printf("%f", value);
    }

   void printBool(bool value) {
    if (value) {
        // \033[32m active le texte vert
        printf("\033[32mtrue\033[0m\n");
    } else {
        // \033[31m active le texte rouge
        printf("\033[31mfalse\033[0m\n");
    }
}
    void printString(char* value)
    {
        printf("%s", value);
    }
    void backSlashN()
    {
        printf("\n");
    } 
}