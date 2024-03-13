#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define try 10

int ClearScanf(int toclear) {
    char c = " ";
    while (c != "\n") {
        c = getchar(toclear);
    }
    return 1;
}


int PrintArray(int array[]) {
    for (int count = 0; count < sizeof(array); count++) {
        printf("%d", array[count]);
        printf(" ");
    }
    printf("\n");
    return 1;
}


int GetInt(int min, int max) {
    int given_value;
    int iError;
    iError = scanf_s("%d", &given_value);
    while (1) {
        if (iError == 0) {
            printf("Please choose an integer between %d", min);
            printf(" & %d", max);
            printf("\n");
            ClearScanf(given_value);
        }
        else {
            return given_value;
        }
    }

}

int GetIntInRange(int minmax[2]) {
    while (1) {
        int min, max;
        while (1) {
            printf("Choose min and max of the number \n");
            printf("min: ");
            int iErrormin = scanf_s("%d", &min);
            printf("max: ");
            int iErrormax = scanf_s("%d", &max);
            if ((min < max && min > 0 && max <= 100) && (iErrormin == 1 && iErrormax == 1)) {
                minmax[0] = min;
                minmax[1] = max;
                return minmax;
            }
            printf("\n");
            ClearScanf(min);
            ClearScanf(max);
        }
    }
}


int EndGame() {
    char retry;
    char possibilities[2][2] = { {'n','N'},{'y','Y'} };

    while (1) {

        printf("Do you want to restart ? \n");
        printf("Y/y or N/n \n");
        scanf_s("%c", &retry, 1);
        for (int count = 0; count < sizeof(possibilities); count++) {
            for (int c = 0; c < sizeof(possibilities[count]); c++) {
                if (retry == possibilities[count][c]) {
                    printf("%d", count);
                    return count;
                }
            }
        }

    }
}



int main()
{
    int play = 1;
    int oldgames[10];
    int gamecount = 0;




    while (play) {
        int win = 0;
        int range[2];
        int given_value, tofind;
        GetIntInRange(range);

        srand(time(NULL));
      
        tofind = rand() % (range[1] + 1 - range[0]) + range[0];

        for (int count = 0; count < try; count++) {

            printf("Choose a number between %d", range[0]);
            printf(" & %d", range[1]);
            printf("\n");
            given_value = GetInt(range[0], range[1]);
            printf("\n");
            if (given_value == tofind) {
                printf("GG\n");
                win = 1;
                break;
            }
            else if (given_value > tofind) {
                printf("More ! \n");
            }
            else { printf("Less ! \n"); }

        }
        if (!win) { printf("bignoob\n"); }

        oldgames[gamecount] = tofind;
        play = EndGame();
        printf("%d", play);
        printf(oldgames);

    }
    return 0;
}
