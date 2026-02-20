#include <cstdio>

extern "C" {

    void printInt(int value) {
        printf("%d\n", value);
    }

    void printFloat(float value) {
        printf("%f\n", value);
    }

    void printBool(bool value) {
        if (value) {
            printf("true\n");
        } else {
            printf("false\n");
        }
    }
    void printString(char* value)
    {
        printf("%s\n", value);
    }

}