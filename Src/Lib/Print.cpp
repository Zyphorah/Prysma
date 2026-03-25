#include <cstdio>
#include <cstdarg> 

extern "C" {
    void print(int type, ...) {
        va_list args;
        va_start(args, type);

        switch (type) {
            case 'i': 
                printf("%d", va_arg(args, int));
                break;

            case 'f': 
                printf("%f", va_arg(args, double)); 
                break;

            case 's': 
                printf("%s", va_arg(args, char*));
                break;

            case 'b': {
                int val = va_arg(args, int);
                if (val != 0) {
                    printf("\033[32mtrue\033[0m");
                } else {
                    printf("\033[31mfalse\033[0m");
                }
            
                break;
            }
        }
        va_end(args);
    }

    void backSlashN() {
        printf("\n");
    } 
}