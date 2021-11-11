#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file_parser.h"

int brute_force(char *truePassword, char **passwordBank, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (strcmp(truePassword, passwordBank[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    char *passwordsFilePath = "passwords_4.txt";
    char *permutationsFilePath = "all_chars_4.txt";
    int nPasswords;
    int nPermutations;

    char **passwords = parseFile(passwordsFilePath, &nPasswords);
    char **permutations = parseFile(permutationsFilePath, &nPermutations);

    // TO REMOVE
    nPasswords = 500;
    // TO REMOVE
    double timeSum = 0.0;
    for (int i = 0; i < nPasswords; i++)
    {
        clock_t start = clock();
        int res = brute_force(passwords[i], permutations, nPermutations);
        clock_t end = clock();
        double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
        timeSum += timeSpent;
        printf("It took %fs to find %s", timeSpent, passwords[i]);
        if (!res)
        {
            printf("Failure\n");
        }
    }
    printf("Average time of %f\n", timeSum / nPasswords);
}
