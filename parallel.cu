#include "gputimer.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


char CHARS_MODE_0[] = {'4', '5', '2', '3', '6', '8', '1', '9', '0', '7'};
#define LENGTH_MODE_0 10

char CHARS_MODE_1[] = {'z', 'w', '4', '7', 'o', '8', '0', 'i', 'r', 's', 'x', '2', '5', 'y', 'q', 'g', '3', 'v', 'c', 'l', 'n', 'u', 'j', '6', '1', 'e', 'b', 't', 'd', 'f', 'a', 'k', 'm', '9', 'p', 'h'};

#define LENGTH_MODE_1 36

char CHARS_MODE_2[] = {'0', 'X', 'D', 'L', 'z', 'y', 'e', 'd', 's', 'R', 'n', 'm', '1', 'B', '8', 'F', 'w', 'V', 'M', 'x', '2', 'j', '4', 'E', 'H', 'a', 'W', 'Z', 'b', 'o', 'r', 'A', 'u', 'U', 'I', 'T', 'q', 'c', 't', 'l', 'g', 'K', '7', '6', 'S', '9', 'O', 'f', 'P', 'J', 'k', '3', 'C', '5', 'v', 'Y', 'p', 'h', 'i', 'Q', 'G', 'N'};

#define LENGTH_MODE_2 62

char CHARS_MODE_3[] = {'m', 'R', 'J', 'D', '#', '6', '9', 'T', 'l', '1', 'V', 'p', 'N', 'i', 'v', 'h', 'q', '*', 'd', 'o', 'B', '3', '4', 'x', 'M', 'r', 'C', 'S', '7', 'u', 'A', 'y', '&', 'f', 'P', 'b', '8', 'z', 'X', 'w', 'E', 'Y', 'j', 'I', 'L', 'g', 'W', '!', '2', 'c', 'n', 'G', '0', 'O', 't', 'K', 'a', '$', 'e', 's', 'H', '?', 'k', 'U', 'Z', '5', 'Q', 'F'};

#define LENGTH_MODE_3 68

//https://stackoverflow.com/questions/19600879/how-to-compare-arrays-of-char-in-cuda-c/19602075#19602075
__device__ int cuda_strcmp(const char* str_a, const char* str_b, int len) {
    int match = 0;
    unsigned i = 0;
    while ((i < len) && (match == 0)) {
        if (str_a[i] != str_b[i]) {
            match = i + 1;
        }
        i++;
    }
    return match;
}

__device__ char *generatePassword(int pwLength, char *chars, int charsLength, int seed)
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

__global__ void bruteForce(char *truePassword, int pwLength, char *chars, int charsLength, volatile int *found, char *foundPassword, long maxIndex)
{
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    int threadCnt = blockDim.x * gridDim.x;

    for (int i = id; i < maxIndex; i += threadCnt)
    {
        if (*found) break;
        char *testPassword = generatePassword(pwLength, chars, charsLength, i);
        if (cuda_strcmp(truePassword, testPassword, pwLength) == 0)
        {
            *found = 1;
            memcpy(foundPassword, testPassword, pwLength * sizeof(char));
        }
        free(testPassword);
    }

}

char** parseFile(char* filePath, int* num, int pwLength)
{
    char* line = (char*)malloc(pwLength * sizeof(char));
    FILE* fp = fopen(filePath, "r");
    int nItems;

    fscanf(fp, "%d", &nItems);
    
    char** items = (char**)malloc(nItems * sizeof(char*));
    int i = 0;

    while (fscanf(fp, "%s", line) != EOF)
    {
        items[i] = (char*)malloc(pwLength * sizeof(char));
        strcpy(items[i], line);
        i++;
    }

    fclose(fp);
    *num = nItems;
    return items;
}

int main(int argc, char *argv[])
{
    // Arguments
    int pwLength = atoi(argv[1]);
    int mode = atoi(argv[2]);
    int threadNum = atoi(argv[3]);
    int blockNum = atoi(argv[4]);

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
    char **passwords = parseFile(passwordsFilePath, &nPasswords, pwLength);
    char* cudaTruePassword, * cudaChars;
    int *found = (int*)malloc(sizeof(int));
    int *cudaFound;

    cudaMalloc((void**)&cudaChars, charsLength * sizeof(char));
    cudaMemcpy(cudaChars, chars, charsLength * sizeof(char), cudaMemcpyHostToDevice);
    cudaMalloc((void**)&cudaTruePassword, pwLength * sizeof(char));
    cudaMalloc((void**)&cudaFound, sizeof(int));  

    // TO REMOVE
    nPasswords = 1000;
    char* foundPw, *cudaFoundPw;
    foundPw = (char*)malloc(pwLength * sizeof(char));
    cudaMalloc((void**)&cudaFoundPw, pwLength * sizeof(char));
    // TO REMOVE
    double timeSum = 0.0;
    for (int i = 0; i < nPasswords; i++)
    {
        *found = 0;
        cudaMemcpy(cudaFound, found, sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(cudaTruePassword, passwords[i], pwLength * sizeof(char), cudaMemcpyHostToDevice);

        GpuTimer timer = GpuTimer();
        timer.Start();
        bruteForce <<<blockNum, threadNum >>> (cudaTruePassword, pwLength, cudaChars, charsLength, cudaFound, cudaFoundPw, (long)pow(charsLength, pwLength));
        cudaDeviceSynchronize();
        timer.Stop();
        timeSum += timer.Elapsed() / 1000.0;
        double currAvg = timeSum / (i + 1);
        cudaMemcpy(found, cudaFound, sizeof(int), cudaMemcpyDeviceToHost);
        cudaMemcpy(foundPw, cudaFoundPw, pwLength * sizeof(char), cudaMemcpyDeviceToHost);
        printf("[%d/%d][avg: %f]: It took %fs to find %s\n", (i + 1), nPasswords, currAvg, timer.Elapsed() / 1000.0, passwords[i]);
        printf("Actually found %s\n", foundPw);
        if (*found == 0)
        {
            printf("Failure\n");
        }
    }
    printf("Average time of %f\n", timeSum / nPasswords);
}
