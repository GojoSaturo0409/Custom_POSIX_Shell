#include"readlines.h"

char **readLines(FILE *file, int *numLines)
{
    char buffer[4097];
    char **lines = NULL;
    int lineCount = 0;

    lines = malloc(sizeof(char *) * 100);

    while (fgets(buffer, sizeof(buffer), file))
    {

        buffer[strcspn(buffer, "\n")] = '\0';

        lines[lineCount] = malloc(4097);

        strncpy(lines[lineCount], buffer, 4097);
        lines[lineCount][4097] = '\0';

        lineCount++;
    }

    lines[lineCount] = NULL;

    if (numLines)
    {
        *numLines = lineCount;
    }

    return lines;
}

// int main() {
//     FILE* file = fopen("example.txt", "r");
//     if (file == NULL) {
//         perror("Failed to open file");
//         return 1;
//     }

//     int numLines = 0;
//     char** lines = readLines(file, &numLines);

//     fclose(file);

//     if (lines) {
//         for (int i = 0; i < numLines; i++) {
//             printf("Line %d: %s\n", i + 1, lines[i]);
//         }

//
//     }

//     return 0;
// }
