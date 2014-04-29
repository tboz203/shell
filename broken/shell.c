#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {/*{{{*/
    char* start;
    enum { WORD, SINGLE, DOUBLE, SEMICOLON, EOL, ERROR } type;
} token;/*}}}*/

void start(char*);
token next();

static char tokenLine[1024];
static char* tokenPosition;

void start(char *line) {/*{{{*/
    /* printf("starting tokenizing\n"); */
    if (line == NULL) {
        fprintf(stderr, "[-] No string passed to tokenizer.\n");
        exit(1);
    }

    strcpy(tokenLine, line);
    tokenPosition = tokenLine;
}/*}}}*/

token next() {/*{{{*/
    /* printf("getting next token\n"); */
    token output;
    if (tokenPosition == NULL || *tokenPosition == '\0') {
        output.start = NULL;
        output.type = EOL;
        return output;
    }

    while (*tokenPosition == ' ' || *tokenPosition == '\t' || *tokenPosition == '\n') {
        tokenPosition++;
    }

    switch (*tokenPosition) {
        case '\0':
        case ';':
            output.start = NULL;
            output.type = EOL;
            return output;
        case '\'':
            tokenPosition++;
            output.start = tokenPosition;
            output.type = SINGLE;

            while (*tokenPosition != '\'' && *tokenPosition != '\0') {
                tokenPosition++;
            }
            break;

        case '\"':
            tokenPosition++;
            output.start = tokenPosition;
            output.type = DOUBLE;

            while (*tokenPosition != '\"' && *tokenPosition != '\0') {
                tokenPosition++;
            }
            break;

        default:
            output.start = tokenPosition;
            output.type = WORD;

            while (*tokenPosition != ' ' && *tokenPosition != '\t' &&
                   *tokenPosition != '\n' && *tokenPosition != '\0') {
                tokenPosition++;
            }
    }

    if (*tokenPosition != '\0') {
        *tokenPosition = '\0';
        tokenPosition++;
    } else if (output.type == SINGLE || output.type == DOUBLE) {
        output.type = ERROR;
    }

    return output;
}/*}}}*/

int main() {/*{{{*/

    char line[1024];
    token current;
    char *tmp;

    for (;;) {
        tmp = fgets(line, 1024, stdin);

        if (tmp == NULL) {
            /* printf("got EOF\n"); */
            break;
        }

        /* printf("fgets loop\n"); */
        start(line);
        for (;;) {
            /* printf("token loop\n"); */
            current = next();
            printf("\t%s\n", current.start);
            if (current.type == EOL) {
                /* printf("type == eol\n"); */
                break;
            }

            if (strcmp(current.start, "exit") == 0) {
                /* printf("got \'exit\'\n"); */
                exit(0);
            }
        }
    }

    return 0;
}/*}}}*/

/* vim: set fdm=marker : */
