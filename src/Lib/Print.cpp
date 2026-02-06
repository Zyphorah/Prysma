#include <cstdio>

extern "C" {
    void printInt() {
        printf("%d\n", 10);
    }

    void printFloat(float x) {
        printf("%f\n", x);
    }
}