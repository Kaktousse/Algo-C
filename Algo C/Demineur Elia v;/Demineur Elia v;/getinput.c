#include "getinput.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <windows.h>



void ClearBuffer()
{
    while (getchar() != '\n');
}

int GetInt() {

    int given_value;
    int iError;
    while (1) {

        iError = scanf_s("%d", &given_value);
        ClearBuffer();
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

int YesOrNo() {

    char retry;
    char possibilities[2][2] = { {'n','N'},{'y','Y'} };

    while (1) {


        printf("Y/y or N/n \n");
        scanf_s("%c", &retry, 2);
        ClearBuffer();
        for (int count = 0; count < sizeof(possibilities); count++) {
            for (int c = 0; c < sizeof(possibilities[count]); c++) {
                if (retry == possibilities[count][c]) {
                    return count;
                }
            }
        }

    }
}

