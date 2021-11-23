#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "file_parser.h"

char CHARS_MODE_0[] = {'4', '5', '2', '3', '6', '8', '1', '9', '0', '7'};
#define LENGTH_MODE_0 10

char CHARS_MODE_1[] = {'z', 'w', '4', '7', 'o', '8', '0', 'i', 'r', 's', 'x', '2', '5', 'y', 'q', 'g', '3', 'v', 'c', 'l', 'n', 'u', 'j', '6', '1', 'e', 'b', 't', 'd', 'f', 'a', 'k', 'm', '9', 'p', 'h'};

#define LENGTH_MODE_1 36

char CHARS_MODE_2[] = {'0', 'X', 'D', 'L', 'z', 'y', 'e', 'd', 's', 'R', 'n', 'm', '1', 'B', '8', 'F', 'w', 'V', 'M', 'x', '2', 'j', '4', 'E', 'H', 'a', 'W', 'Z', 'b', 'o', 'r', 'A', 'u', 'U', 'I', 'T', 'q', 'c', 't', 'l', 'g', 'K', '7', '6', 'S', '9', 'O', 'f', 'P', 'J', 'k', '3', 'C', '5', 'v', 'Y', 'p', 'h', 'i', 'Q', 'G', 'N'};

#define LENGTH_MODE_2 62

char CHARS_MODE_3[] = {'m', 'R', 'J', 'D', '#', '6', '9', 'T', 'l', '1', 'V', 'p', 'N', 'i', 'v', 'h', 'q', '*', 'd', 'o', 'B', '3', '4', 'x', 'M', 'r', 'C', 'S', '7', 'u', 'A', 'y', '&', 'f', 'P', 'b', '8', 'z', 'X', 'w', 'E', 'Y', 'j', 'I', 'L', 'g', 'W', '!', '2', 'c', 'n', 'G', '0', 'O', 't', 'K', 'a', '$', 'e', 's', 'H', '?', 'k', 'U', 'Z', '5', 'Q', 'F'};

#define LENGTH_MODE_3 68

char *generatePassword(int pwLength, char *chars, int charsLength, int seed)
{
    int quotient = seed;
    int remainder;
    char *pw = (char *)malloc(pwLength * sizeof(char));
    for (int i = pwLength - 1; i >= 0; i--)
    {
        remainder = quotient % charsLength;
        quotient = quotient / charsLength;
        char c = chars[remainder];
        pw[i] = c;
    }
    pw[pwLength] = '\n';
    return pw;
}

int bruteForce(char *truePassword, int pwLength, char *chars, int charsLength)
{
    long maxIndex = (long)pow(charsLength, pwLength);
    for (int i = 0; i < maxIndex; i++)
    {
        char *testPassword = generatePassword(pwLength, chars, charsLength, i);
        if (strcmp(truePassword, testPassword) == 0)
        {
            return 1;
        }
        free(testPassword);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Arguments
    int pwLength = atoi(argv[1]);
    int mode = atoi(argv[2]);

    char *chars;
    int charsLength;
    char passwordsFilePath[100];
    char temp[2];
    strcpy(passwordsFilePath, "./passwords/");
    switch (mode)
    {
    case 0:
        chars = CHARS_MODE_0;
        charsLength = LENGTH_MODE_0;
        temp[0] = pwLength + '0';
        temp[1] = '\0';
        strcat(passwordsFilePath, "digits_passwords_");
        strcat(passwordsFilePath, temp);
        strcat(passwordsFilePath, ".txt");
        break;
    case 1:
        chars = CHARS_MODE_1;
        charsLength = LENGTH_MODE_1;
        temp[0] = pwLength + '0';
        temp[1] = '\0';
        strcat(passwordsFilePath, "lc_alphanum_passwords_");
        strcat(passwordsFilePath, temp);
        strcat(passwordsFilePath, ".txt");
        break;
    case 2:
        chars = CHARS_MODE_2;
        charsLength = LENGTH_MODE_2;
        temp[0] = pwLength + '0';
        temp[1] = '\0';
        strcat(passwordsFilePath, "alphanum_passwords_");
        strcat(passwordsFilePath, temp);
        strcat(passwordsFilePath, ".txt");
        break;
    case 3:
        chars = CHARS_MODE_3;
        charsLength = LENGTH_MODE_3;
        temp[0] = pwLength + '0';
        temp[1] = '\0';
        strcat(passwordsFilePath, "all_passwords_");
        strcat(passwordsFilePath, temp);
        strcat(passwordsFilePath, ".txt");
        break;
    }

    // Set up the test password list
    int nPasswords;
    char **passwords = parseFile(passwordsFilePath, &nPasswords);

    // TO REMOVE
    nPasswords = 1000;
    // TO REMOVE
    double timeSum = 0.0;
    for (int i = 0; i < nPasswords; i++)
    {
        clock_t start = clock();
        int res = bruteForce(passwords[i], pwLength, chars, charsLength);
        clock_t end = clock();
        double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
        timeSum += timeSpent;
        double currAvg = timeSum / (i + 1);
        printf("[%d/%d][avg: %f]: It took %fs to find %s", (i + 1), nPasswords, currAvg, timeSpent, passwords[i]);
        if (!res)
        {
            printf("Failure\n");
        }
    }
    printf("Average time of %f\n", timeSum / nPasswords);
}
