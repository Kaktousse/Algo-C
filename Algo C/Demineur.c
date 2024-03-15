#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 


typedef struct Grid
{
    int size;
    int Bomb;
    int flag;
};

typedef struct Tile
{
    int Bomb;
    int BombAround;
    int revealed; 
    int flag;

} Tile;

void ClearBuffer()
{
    while (getchar() != '\n');
}

void ChangeTile(Tile** grid, int lign, int column) {
    grid[lign][column].revealed = 1;
    
}

void SetBombAround(Tile** grid, int setlign, int setcolumn, int size) {
    for (int lign = -1; lign <= 1; lign++) {
        for (int column = -1; column <= 1; column++) {
            if ((lign != 0 && column != 0) && (setlign+lign > -1 && setlign+lign < size) && (setcolumn + column > -1 && setcolumn + column < size) ) {
                if (grid[setlign + lign][setcolumn + column].Bomb == 1) {
                    grid[setlign][setcolumn].BombAround += 1;
                }
            }
        }
    }
}

void AskCoor(Tile** grid) {
    int lign, column;
    printf("Choose the lign : ");
    scanf_s("%d", &lign);
    ClearBuffer();
    printf("Choose the column : ");
    scanf_s("%d", &column);
    ClearBuffer();

    ChangeTile(grid, lign, column);
    CheckTile(grid, lign, column);

}


int CheckTile(Tile** grid, int lign, int column) {
    return 1;
}



int** PlaceBomb(Tile** grid, int size, int bombnumber) {
    
    int place = 0;
    int* bomb = (int*) malloc(sizeof(int)*2);
    for (int count = 0; count < bombnumber; count++) {
        while (!place) {
            bomb[0] = rand() % size;
            bomb[1] = rand() % size;
            if (grid[bomb[0]][bomb[1]].Bomb == 0) {
                grid[bomb[0]][bomb[1]].Bomb = 1;
                place = 1;
            }
        }
        place = 0;
    }
    
    for (int lign = 0; lign < size; lign++) {
        for (int column = 0; column < size; column++) {
            SetBombAround(grid, lign, column, size);
        }
    }

}


Tile** InitGrid(int givensize) {
    
    int x, y; 
    Tile** grid;

    grid = (Tile*)malloc(sizeof(Tile) * givensize);

    for (int count = 0; count < givensize; count++) {
        grid[count] = (Tile*) malloc(givensize * sizeof(Tile));
        for (int i = 0; i < givensize; i++) {
            grid[count][i].Bomb = 0;
            grid[count][i].flag = 0;
            grid[count][i].revealed = 0;
            grid[count][i].BombAround = 0;
        }
    }

    return grid;
}


void PrintGrid(Tile** grid, int size) {
    printf("\t");
    for (int i = 0; i < size; i++) {
        if (i < 10) { printf("   %d", i);  } else { printf("  %d", i); }
       
    }
    printf("\n");

    for (int lign = 0; lign < size; ++lign) {

        printf(" %d \t", lign);
        for (int column = 0; column < size; ++column) {
            if (grid[lign][column].revealed == 0) {
                printf(" | []");
            }else if (grid[lign][column].BombAround == 0) {
                printf(" |   "); 
            }
            else {
                printf(" | %d", grid[lign][column].BombAround);
            }
            
        }
        printf(" | \n");
    }
}


int main() {

    struct Grid myGrid;
    myGrid.size = 10;
    myGrid.Bomb = 10;


    srand(time(NULL));
    Tile** grid = InitGrid(myGrid.size);
    PlaceBomb(grid, myGrid.size, myGrid.Bomb);
    PrintGrid(grid, myGrid.size);
    AskCoor(grid);
    PrintGrid(grid, myGrid.size);
    return 1;
}