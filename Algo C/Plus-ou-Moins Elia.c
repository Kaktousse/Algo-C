#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define try 10

void ClearScanf() {
    char c = " ";
    while (c != '\n') {
        c = getchar();
    }
}


void PrintArray(int* array, int arraysize) {
    for (int count = 0; count < arraysize; ++count) {
        printf("%d\n", array[count]);
    }
}




int GetInt() {
    int given_value;
    int iError;
    while (1) {

        iError = scanf_s("%d", &given_value);
        ClearScanf();
        if (iError == 1) {
            return given_value;
        }
        printf("Please choose an integer");
        printf("\n");
    }

}

int GetIntInRange(int min, int max) {
    int given_value;
    while (1) {

        given_value = GetInt();
        if (given_value <= max && given_value >= min) {
            return given_value;
        }
        printf("Please choose an integer between %d", min);
        printf(" & %d", max);
        printf("\n");

    }

}

void GetRange(int minmax[2]) {

    printf("Choose min and max of the number \n");
    printf("min: ");
    minmax[0] = GetIntInRange(1, 99);
    printf("max: ");
    minmax[1] = GetIntInRange(minmax[0] + 1, 100);
    printf("\n");
}


int EndGame() {
    char retry;
    char possibilities[2][2] = { {'n','N'},{'y','Y'} };

    while (1) {

        printf("Do you want to restart ? \n");
        printf("Y/y or N/n \n");
        scanf_s("%c", &retry, 2);
        ClearScanf();
        for (int count = 0; count < sizeof(possibilities); count++) {
            for (int c = 0; c < sizeof(possibilities[count]); c++) {
                if (retry == possibilities[count][c]) {
                    return count;
                }
            }
        }

    }
}





int main()
{
    int play = 1;
    int gamecount = 0;
    int* oldgames = (int*)malloc(sizeof(int));
    if (oldgames == NULL)
        exit(1);

    while (play) {
        int win = 0;
        int range[2];
        int given_value, tofind;
        srand(time(NULL));
        GetRange(range);

        tofind = (rand() % (range[1] - range[0] + 1)) + range[0];

        int* pTmp = (int*)realloc(oldgames, (size_t)(sizeof(int) * (gamecount + 1)));
        if (pTmp == NULL)
        {
            free(pTmp);
            exit(1);
        }

        oldgames = pTmp;

        oldgames[gamecount] = tofind;
        gamecount += 1;


        for (int count = 0; count < try; count++) {

            printf("Choose a number between %d", range[0]);
            printf(" & %d", range[1]);
            printf("\n");
            given_value = GetIntInRange(range[0], range[1]);
            printf("\n");
            if (given_value == tofind) {
                printf("GG\n");
                win = 1;
                break;
            }
            else if (given_value < tofind) {
                printf("More ! \n");
            }
            else { printf("Less ! \n"); }

        }
        if (!win) { printf("bignoob\n"); }

        play = EndGame();

    }
    printf("\n");
    PrintArray(oldgames, gamecount);
    free(oldgames);

    return 0;
}
