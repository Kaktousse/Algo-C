#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100
#define NUMS_TO_GENERATE 1

void ClearBuffer()
{
    while (getchar() != '\n');
}

int DefineRange(int range[2]) {
    int min, MAX;

    printf("Set the min : ");
    scanf_s("%d", &min);
    ClearBuffer();
    printf("Set the max : ");
    scanf_s("%d", &MAX);
    ClearBuffer();

    range[0] = min;
    range[1] = MAX;

    return min, MAX;
}

int Game()
{
    int given_value, rdm_number, tries, left_try, min, MAX;
    int range[2];
    srand(time(NULL));

    DefineRange(range);
    min = range[0];
    MAX = range[1];

    tries = 10;
    left_try = 10;

    rdm_number = rand() % MAX + 1;

    for (int count = 0; count < tries; count++)
    {
        printf("Choose a number between %d and %d :", min, MAX);
        scanf_s("%d", &given_value);
        ClearBuffer();

        if (given_value < rdm_number)
        {
            printf("it is more ! \n");
            min = given_value + 1;
        }

        if (given_value > rdm_number)
        {
            printf("it is less ! \n");
            MAX = given_value - 1;
        }

        if (given_value == rdm_number)
        {
            printf("GG ! Well played !");
            return 0;
        }
        left_try--;
        printf("tries left : %d \n", left_try);
    }

    printf("Looser !");
    return 0;
}

int Replay()
{
    char answer;
    answer = ' ';

    while (1)
    {
        printf("\n Play again? Y/y or N/n : ");
        scanf_s("%c", &answer, 1);
        ClearBuffer();

        if (answer == 'Y' || answer == 'y')
        {
            return 1;
        }

        if (answer == 'N' || answer == 'n')
        {
            return 0;
        }
    }
}

void GuessTable()
{

}

int main()
{
    Game();
    while (1)
    {
        if (Replay())
        {
            Game();
        }
        else
        {
            return 0;
        }
    }
}