#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "file_parser.h"

char CHARS_MODE_0[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define LENGTH_MODE_0 10

char CHARS_MODE_1[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

#define LENGTH_MODE_1 36

char CHARS_MODE_2[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

#define LENGTH_MODE_2 62

char CHARS_MODE_3[] =
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '!', '?', '$', '#', '&', '*'};

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
    nPasswords = 500;
    // TO REMOVE
    double timeSum = 0.0;
    for (int i = 0; i < nPasswords; i++)
    {
        clock_t start = clock();
        int res = bruteForce(passwords[i], pwLength, chars, charsLength);
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
