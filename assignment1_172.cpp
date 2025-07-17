#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *inputFile, *outputFile;
    char ch;
    int inComment = 0;
    int inString = 0;

    inputFile = fopen("input.c", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input.c\n");
        return 1;
    }

    outputFile = fopen("output.c", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output.c\n");
        fclose(inputFile);
        return 1;
    }

    while ((ch = fgetc(inputFile)) != EOF) {
        if (inString) {
            fputc(ch, outputFile);
            if (ch == '"') {
                inString = 0;
            } else if (ch == '\\') {
                ch = fgetc(inputFile);
                if (ch != EOF) {
                    fputc(ch, outputFile);
                }
            }
            continue;
        }

        if (inComment == 1) {
            if (ch == '\n') {
                inComment = 0;
            }
            continue;
        }

        if (inComment == 2) {
            if (ch == '*') {
                ch = fgetc(inputFile);
                if (ch == '/') {
                    inComment = 0;
                } else {
                    ungetc(ch, inputFile);
                }
            }
            continue;
        }

        if (ch == '/') {
            char nextCh = fgetc(inputFile);
            if (nextCh == '/') {
                inComment = 1;
                continue;
            } else if (nextCh == '*') {
                inComment = 2;
                continue;
            } else {
                fputc(ch, outputFile);
                ungetc(nextCh, inputFile);
                continue;
            }
        }

        if (ch == '"') {
            inString = 1;
            fputc(ch, outputFile);
            continue;
        }

        if (ch == ' ' || ch == '\t' || ch == '\n') {
            char nextCh = fgetc(inputFile);
            if (nextCh != ' ' && nextCh != '\t' && nextCh != '\n') {
                fputc(' ', outputFile);
            }
            ungetc(nextCh, inputFile);
            continue;
        }

        fputc(ch, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    outputFile = fopen("output.c", "r");
    if (outputFile == NULL) {
        printf("Error: Could not open output.c for reading.\n");
        return 1;
    }

    printf("Processed output:\n");
    while ((ch = fgetc(outputFile)) != EOF) {
        printf("%c", ch);
    }
    printf("\n");

    fclose(outputFile);

    return 0;
}
