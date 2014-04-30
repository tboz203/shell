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
    long value = 0;
    char *end;

    if (statement[1] != NULL) {
        value = strtol(statement[1], &end, 0);
        if (*end != '\0') {
            printf("[-] Error: '%s' is not a number\n", statement[1]);
            return 1;
        }
    }

    exit(value);
    return 0;
}

int doCD (char *statement[]) {
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

    if (statement[0] == NULL) {
        return;
    }

    if (strcmp(statement[0], "exit") == 0) {
        if (doEXIT(statement)) { return; }
    } else if (strcmp(statement[0], "cd") == 0) {
        if (doCD(statement)) { return; }
    }

    if (fork() == 0) {
        // we're the child
        execvp(statement[0], statement);
        localerr = errno;
        printf("[-] Error: %s\n", strerror(localerr));
    } else {
        wait(NULL);
    }
}

int main() {

    token current;
    char *tmp;
    char *statement[256];
    int index;
    char cwd[1024];

    for (;;) {

        getwd(cwd);
        printf("%s$ ", cwd);
        fflush(stdout);

        tmp = fgets(line, 1024, stdin);
        index = 0;

        if (tmp == NULL) {
            break;
        }

        position = line;
        for (;;) {
            current = next(tmp);
            if (current.type == EOL) {
                break;
            }

            process(index, current, statement);
            index++;
        }

        statement[index] = NULL;
        execute(statement);

        // cleanup
        for (index = 0; index < 256; index++) {
            statement[index] = NULL;
        }

        for (index = 0; index < 1024; index++) {
            line[index] = '\0';
        }
    }

    return 0;
}
