#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 
#include<conio.h>



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


void SetBombAround(Grid grid, int setlign, int setcolumn, int size) {
    for (int lign = -1; lign < 2; lign++){
        for (int column = -1; column <2; column++){
            grid.tiles[setlign+lign][setcolumn+column].BombAround +=1;
        }
    }
}

void AskCoor(Grid grid, int firststart) {
    int lign, column;
    printf("Choose the lign : ");
    scanf_s("%d", &lign);
    ClearBuffer();
    printf("Choose the column : ");
    scanf_s("%d", &column);
    ClearBuffer();

    if (firststart){
        PlaceBomb(grid);
    }
    grid.tiles[lign][column].revealed = 1;

    CheckTile(grid, lign, column, 1);

}


int CheckTile(Grid grid, int lign, int column, int expand) {
    return 1;
}



int** PlaceBomb(Grid grid) {
    int size = grid.size;

    int place = 0;
    int* bomb = (int*) malloc(sizeof(int)*2);
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


void PrintGrid(Grid grid) {

    int size = grid.size;
    printf("Press F to place a flag");
    printf("\t");
    for (int i = 0; i < size; i++) {
        if (i < 10) { printf("   0%d", i);  } else {printf("   %d,i");}
       
    }
    printf("\n");

    for (int lign = 0; lign < size; ++lign) {

        printf(" %d \t", lign);
        for (int column = 0; column < size; ++column) {
            if (grid.tiles[lign][column].revealed == 0) {
                printf(" | []");
            }else if (grid.tiles[lign][column].BombAround == 0) {
                printf(" |   "); 
            }
            else {
                printf(" | %d ", grid.tiles[lign][column].BombAround);
            }
            
        }
        printf(" | \n");
    }
}


int main() {
    printf("aaaa");
    struct Grid myGrid;
    myGrid.size = 10;
    myGrid.Bomb = 10;
    srand(time(NULL));
    int firststart = 1;

    myGrid.tiles = InitGrid(myGrid.size);
    PrintGrid(myGrid);
    //first round
    AskCoor(myGrid, firststart);
    PrintGrid(myGrid);

    if (kbhit){
        char ch = getch();
        if(ch = "E"){
            printf("ssss");
            PlaceFlag();
        }
    }

    return 1;
}