#include <stdio.h>
#include <stdlib.h>

int main() {
    /* printf("%s\n", "Hello World!"); */
    char array[256];
    char *pointer = array;

    for (;;) {
        gets(pointer);
        system(pointer);
    }

    return 0;
}
