#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    char command[256];
    char statement[256][256];
    char *curr;
    char *holding;
    char saved;
    int index;

    /* for (;;) { */
        printf("getting input\n");
        gets(command);
        index = 0;

        holding = curr = command;
        while (*curr != '\0') {
            printf("curr != 0");
            while (*curr != ' ' && *curr != '\n' && *curr != '\0') {
                curr++;
            }
            saved = *curr;
            *curr = '\0';
            printf("holding = %s\n", holding);
            strcpy(statement[index], holding);
            *curr = saved;
            holding = curr;
            index++;
        }

    /* } */

    return 0;
}
