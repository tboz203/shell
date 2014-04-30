#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

typedef struct {
    char* start;
    enum { WORD, SINGLE, DOUBLE, SEMICOLON, EOL, ERROR } type;
} token;

void start(char*);
token next();

char line[1024];
char *position;
int localerr;

token next() {
    fprintf(stderr, "getting next token\n");
    token output;
    if (position == NULL || *position == '\0') {
        output.start = NULL;
        output.type = EOL;
        return output;
    }

    while (*position == ' ' || *position == '\t' || *position == '\n') {
        position++;
    }

    switch (*position) {
        case '\0':
        case ';':
            output.start = NULL;
            output.type = EOL;
            return output;
        case '\'':
            position++;
            output.start = position;
            output.type = SINGLE;

            while (*position != '\'' && *position != '\0') {
                position++;
            }
            break;

        case '\"':
            position++;
            output.start = position;
            output.type = DOUBLE;

            while (*position != '\"' && *position != '\0') {
                position++;
            }
            break;

        default:
            output.start = position;
            output.type = WORD;

            while (*position != ' ' && *position != '\t' &&
                   *position != '\n' && *position != '\0') {
                position++;
            }
    }

    if (*position != '\0') {
        *position = '\0';
        position++;
    } else if (output.type == SINGLE || output.type == DOUBLE) {
        output.type = ERROR;
    }

    return output;
}

void process(int index, token current, char *statement[]) {
    statement[index] = current.start;
}

int doEXIT (char *statement[]) {
    int value = 0;
    if

int doCD (char *statment[]) {
    char* dirname;
    if (statement[1] == NULL) {
        dirname = getenv("HOME");
    } else {
        dirname = statement[1];
    }

    if (chdir(dirname) == -1) {
        localerr = errno;
        printf("[-] Error: %s\n", strerror(localerr));
    }
    return 1;
}

void execute(char *statement[]) {

    if (strcmp(statement[0], "exit") == 0) {
        if (doEXIT(statement)) { return; }
    } else if (strcmp(statement[0], "cd") == 0) {
        if (doCD(statement)) { return; }
    }

    if (fork() == 0) {
        // we're the child
        execvp(statement[0], statement);
        localerr = errno;
        printf("[-] Error: ");
        printf("%s\n", strerror(localerr));
    } else {
        wait(NULL);
    }
}

int main() {

    token current;
    char *tmp;
    char *statement[256];
    int index;

    for (;;) {
        tmp = fgets(line, 1024, stdin);
        index = 0;

        if (tmp == NULL) {
            fprintf(stderr, "got EOF\n");
            break;
        }

        fprintf(stderr, "fgets loop\n");
        position = line;
        for (;;) {
            fprintf(stderr, "token loop\n");
            current = next(tmp);
            fprintf(stderr, "\t%s\n", current.start);
            if (current.type == EOL) {
                fprintf(stderr, "type == eol\n");
                break;
            }

            process(index, current, statement);
            index++;
        }

        statement[index] = NULL;
        execute(statement);
    }

    return 0;
}
