#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {

    char command[256];
    char *statement[256];
    char *curr;
    int index;
    int localerror;

    for (;;) {
        /* printf("getting input\n"); */

        printf("$$ ");
        fflush(stdout);
        fgets(command, 1024, stdin);

        /* printf("command = %s\n", command); */
        index = 0;

        curr = command;
        for (;;) {
            // skip initial whitespace
            while (*curr == ' ' || *curr == '\n') {
                curr++;
            }
            // if no word, break
            if (curr == '\0') {
                break;
            }
            // save the start of this substring
            statement[index] = curr;
            // go until the end of this word (not worried about tokenizing here)
            while (*curr != ' ' && *curr != '\n' && *curr != '\0') {
                curr++;
            }
            // if didn't find the end of the string,
            if (*curr != '\0') {
                // mark it
                *curr = '\0';
                /* printf("statement[%d] = %s\n", index, statement[index]); */
                // move to the next word
                curr++;
                index++;
            } else {
                // if we did find the end of the string,
                /* printf("statement[%d] = %s\n", index, statement[index]); */
                statement[index+1] = NULL;
                break;
            }
        }

        if (fork() == 0) {
            // we're the child
            execvp(statement[0], statement);
            localerror = errno;
            printf("[-] Error: ");
            printf("%s\n", strerror(localerror));
        } else {
            wait(NULL);
        }

    }

    return 0;
}
