#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 
#include<conio.h>
#include <windows.h>


typedef struct Tile
{
    int Bomb;
    int BombAround;
    int revealed;
    int flag;

} Tile;

typedef struct Grid
{
    Tile** tiles;
    int size;
    int Bomb;
    int flag;
} Grid;







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

int Retry() {

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




int CheckWin(Grid grid) {
    for (int lign = 0; lign < grid.size; lign++) {
        for (int column = 0; column < grid.size; column++) {
            if (grid.tiles[lign][column].revealed == 0 && grid.tiles[lign][column].Bomb == 0) {
                return 0;
            }
        }
    }
    return 1;
}


void Clear() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
}






int PlaceFlag(Grid grid) {
    int lign, column;
    printf("\nChoose where to place the Flag\n");
    while (1) {
            printf("Choose -1 to cancel\n");
            printf("Choose the lign : ");
            lign = GetIntInRange(-1, grid.size);
            printf("\nChoose the column : ");
            column = GetIntInRange(-1, grid.size);
            if (lign == -1 || column == -1) {
                return 1;
            }

            if (grid.tiles[lign][column].flag == 1){
                printf("Do you want to remove the flag");
                int remove = Retry();
                if (remove){
                    grid.tiles[lign][column].flag = 0;
                    
                }

            }

            if (grid.tiles[lign][column].revealed == 0) {
                grid.tiles[lign][column].flag = 1;
                return 1;
            }

            printf("Please Choose a non-revealed tile\n");

    }
}

void SetBombAround(Grid grid, int setlign, int setcolumn, int size) {
    for (int lign = -1; lign < 2; lign++) {
        for (int column = -1; column < 2; column++) {
            if( (setlign + lign) > -1 && (setlign + lign) < grid.size && (setcolumn + column)>-1 && (setcolumn + column)<size )
            grid.tiles[setlign + lign][setcolumn + column].BombAround += 1;
        }
    }
}

void CheckEmptyTiles(Grid grid, int setlign, int setcolumn) {
    for (int lign = -1; lign < 2; lign++) {
        for (int column = -1; column < 2; column++) {
            if ((setlign + lign) > -1 && (setlign + lign) < grid.size && (setcolumn + column) > -1 && (setcolumn + column) < grid.size) {

                if (grid.tiles[setlign + lign][setcolumn + column].revealed == 0 && grid.tiles[setlign + lign][setcolumn + column].Bomb == 0 && grid.tiles[setlign + lign][setcolumn + column].flag ==0 ) {
                    grid.tiles[setlign + lign][setcolumn + column].revealed = 1;
                    if (grid.tiles[setlign + lign][setcolumn + column].BombAround == 0) {
                        CheckEmptyTiles(grid, setlign + lign, setcolumn + column);
                    }
                }
            }
        }
    }
}



int PlaceBomb(Grid grid) {
    int size = grid.size;

    int place = 0;
    int* bomb = (int*)malloc(sizeof(int) * 2);
    for (int count = 0; count < grid.Bomb; count++) {
        while (!place) {
            bomb[0] = rand() % size;
            bomb[1] = rand() % size;
            if (grid.tiles[bomb[0]][bomb[1]].Bomb == 0) {
                grid.tiles[bomb[0]][bomb[1]].Bomb = 1;
                place = 1;
                SetBombAround(grid, bomb[0], bomb[1], size);
            }
        }
        place = 0;
    }
    return 1;

}

int AskCoor(Grid grid, int firststart) {
    int lign, column,flag;

    printf("Choose the lign : ");
    scanf_s("%d", &lign);
    ClearBuffer();
    printf("Choose the column : ");
    scanf_s("%d", &column);
    ClearBuffer();

    grid.tiles[lign][column].revealed = 1;
    
    if (firststart == 1) {
        PlaceBomb(grid);
    }

    if (grid.tiles[lign][column].Bomb == 1) {
        return 0;
    }

    CheckEmptyTiles(grid, lign, column);
    return 1;
}




Tile** InitGrid(int givensize) {

    int x, y;
    Tile** grid;

    grid = (Tile**)malloc(sizeof(Tile) * givensize);

    for (int count = 0; count < givensize; count++) {
        grid[count] = (Tile*)malloc(givensize * sizeof(Tile));
        for (int i = 0; i < givensize; i++) {
            grid[count][i].Bomb = 0;
            grid[count][i].flag = 0;
            grid[count][i].revealed = 0;
            grid[count][i].BombAround = 0;
        }
    }

    return grid;
}


void PrintGrid(Grid grid) {


    int size = grid.size;
    printf("Press F to place a flag or Choose the tile to reveal.\n ");
    printf("\t");
    for (int i = 0; i < size; i++) {
        if (i < 10) { printf("   0%d", i); }
        else { printf("   %d",i); }

    }
    printf("\n");

    for (int lign = 0; lign < size; ++lign) {

        printf(" %d \t", lign);
        for (int column = 0; column < size; ++column) {
            if (grid.tiles[lign][column].flag == 1) {
                printf(" | \x1B[32mI \x1B[0m");
            }
            else if (grid.tiles[lign][column].revealed == 0) {
                printf(" | - ");
            }
            else if (grid.tiles[lign][column].BombAround == 0) {
                printf(" |   ");
            }
            else if (grid.tiles[lign][column].Bomb == 1) {
                printf(" | \x1B[31mO \x1B[0m");
            }
            else {
                printf(" | \x1B[3%dm%d \x1B[0m", (grid.tiles[lign][column].BombAround+2), grid.tiles[lign][column].BombAround);
            }

        }
        printf(" | \n");
    }
}



void EndGame(Grid grid) {
    for (int lign = 0; lign < grid.size; lign++) {
        for (int column = 0; column < grid.size; column++) {
            if (grid.tiles[lign][column].Bomb == 1) {
                grid.tiles[lign][column].revealed = 1;
            }
        }
    }
}



int main() {


    int play = 1;
    struct Grid myGrid;
    printf("Choose the size of the grid :");
    myGrid.size = GetIntInRange(2, 100);
    printf("Choose the number of mine :");
    myGrid.Bomb = GetIntInRange(1, myGrid.size*myGrid.size - 1);
    srand(time(NULL));
    int firststart = 1;
    int loose = 1;
    myGrid.tiles = InitGrid(myGrid.size);

    while (play) {
        Clear();
        PrintGrid(myGrid);
        if (_kbhit) {
            if (_getch() == 'f') {
                PlaceFlag(myGrid);
            }
            else {
                loose = AskCoor(myGrid, firststart);
                firststart = 0;
                if (CheckWin(myGrid)) {
                    printf("You Win ! ");
                    printf("\n");
                    play = 0;
                }
                if (!loose) {
                    printf("You Loose ! ");
                    printf("\n");
                    play = 0;
                }
            }
        }
    }
    EndGame(myGrid);
    printf("\n");
    PrintGrid(myGrid);
    printf("Do you want to restart ? \n");
    if (Retry()) {
        main();
    }
    return 0;
}