/* wordList.txt */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define ResultGreen 1
#define ResultYellow 2
#define ResultRed 4
#define max 5195

struct s_words
{
    char **arr;
    int n;
};

typedef struct s_words Words;
typedef char Result;

void Example_print_result(Result[]);
Result *check_word(char *, char *);
bool isin(char, char *);
Result check_char(char, int, char *);
int main(int, char **);

void Example_print_result(Result res[])
{
    int i;

    for (i = 0; i < 5; i++)
        switch (res[i])
        {
        case ResultGreen:
            printf("Green\n");
            break;
        case ResultYellow:
            printf("Yellow\n");
            break;
        case ResultRed:
            printf("Red\n");
            break;
        default:
            printf("something went wrong. Value of res[i]: %d\n", res[i]);
            break;
        }
}

Result *check_word(char *guess, char *word)
{
    static Result res[5];
    int i;

    for (i = 0; i < 5; i++)
    {
        res[i] = check_char(guess[i], i, word);
    }

    return res;
}

bool isin(char c, char *str)
{
    bool result = false;
    int i, size;

    size = strlen(str);

    for (i = 0; i < size; i++)
    {
        if (str[i] == c)
        {
            result = true;
            break;
        }
    }

    return result;
}

Result check_char(char guess, int index, char *word)
{
    char correct = word[index];

    if (guess == correct)
        return ResultGreen;
    else if (isin(guess, word))
        return ResultYellow;
    else
        return ResultRed;
}

Words read_file(char *filename)
{
    char buff[8];
    int i, size;
    FILE *fd;
    char **ret;

    fd = fopen(filename, "r");
    if (!fd)
    {
        perror("fopen");
        Words words = {
            .arr = (char **)0,
            .n = 0};
        return words;
    }

    size = max * 5; // 5 bites per variable
    ret = (char **)malloc(size);
    if (!ret)
    {
        fclose(fd);
        perror("malloc");
        Words words = {
            .arr = (char **)0,
            .n = 0};
        return words;
    }

    i = 0;

    memset(buff, 0, 8);
    while (fgets(buff, 7, fd))
    {
        size = strlen(buff);
        if (size < 1)
        {
            memset(buff, 0, 8);
            continue;
        }

        // nylon\n\0
        // 012345
        // strlen() => 6

        size--;
        buff[size] = 0; // removes \n

        if (size != 5)
        {
            memset(buff, 0, 8);
            continue;
        }

        ret[i][0] = buff[0];
        ret[i][1] = buff[1];
        ret[i][2] = buff[2];
        ret[i][3] = buff[3];
        ret[i][4] = buff[4];

        memset(buff, 0, 8);
        i++;

        if (max <= i)
        {
            break;
        }
    }

    fclose(fd);
    Words words = {
        .arr = ret,
        .n = i};

    return words;
}

int main(int argc, char *argv[])
{
    char *correct, *guess;
    Result *res;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s GUESSWORD CORRECTWORD\n", argv[0]);
        return -1;
    }

    guess = argv[1];
    correct = argv[2];

    res = check_word(guess, correct);
    Example_print_result(res);

    return 0;
}