#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **parseFile(char *filePath, int *num)
{
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen(filePath, "r");
    int nItems;

    getline(&line, &len, fp);
    nItems = atoi(line);
    char **items = malloc(nItems * sizeof(char *));
    int i = 0;
    while (getline(&line, &len, fp) != -1)
    {
        items[i] = malloc(15 * sizeof(char));
        strcpy(items[i], line);
        i++;
    }

    fclose(fp);

    *num = nItems;
    return items;
}