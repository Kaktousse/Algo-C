
#include "grid.h"
#include "getinput.h"

#include <stdlib.h> 
#include <stdio.h> 
#include <windows.h>
#include <time.h> 


int CheckWin(const Grid* grid) {
    if (grid->remainingTiles == 0)
        return TRUE;
    return FALSE;
}

int PlaceFlag(Grid* grid,Coord coord ) {

    if (grid->tiles[coord.x][coord.y].flag == 1) {
        RemoveFlag(grid, coord);
        return 0;
    }
    if (grid->tiles[coord.x][coord.y].revealed == 1) {
        return 0;
    }

    grid->flag -= 1;
    grid->tiles[coord.x][coord.y].flag = 1;

    return 1;
}

void RemoveFlag(Grid* grid, Coord coord) {
    grid->tiles[coord.x][coord.y].flag = 0;
    grid->flag += 1; 
}

void SetBombAround(Grid* grid, int setlign, int setcolumn) {
    SDL_Color text_color[8] = { {228,255,0}, {0,169,113},{30,101,168},{154,59,255}, {207,148,67}, {224,65,0},{255,132,193},{255,57,57} };
    for (int lign = -1; lign < 2; lign++) {
        for (int column = -1; column < 2; column++) {
            if ((setlign + lign) > -1 && (setlign + lign) < grid->size && (setcolumn + column) > -1 && (setcolumn + column) < grid->size) {
                grid->tiles[setlign + lign][setcolumn + column].bombAround += 1;
                grid->tiles[setlign + lign][setcolumn + column].color = text_color[grid->tiles[setlign + lign][setcolumn + column].bombAround];
            }
        }
    }
    
}

Tile* GetTile(Grid* grid, int x, int y)
{
    if (x < 0 || y < 0 || x >= grid->size || y >= grid->size)
        return NULL;

    return &grid->tiles[x][y];
}

void RevealTile(Grid* grid, int setlign, int setcolumn, BOOL firststart) {

    Tile* tile = GetTile(grid, setlign, setcolumn);

    if (tile == NULL)
        return;

    if (tile->revealed)
        return;

    if (tile->isBomb)
        return;

    if (tile->flag)
        return;

    tile->revealed = 1;

    grid->remainingTiles -= 1;

    if (!firststart && tile->bombAround > 0)
        return;

    firststart = FALSE;

    for (int lign = -1; lign < 2; lign++) {
        for (int column = -1; column < 2; column++) {
            RevealTile(grid, setlign + lign, setcolumn + column, firststart);
        }
    }
}

BOOL SafeZone(Grid* grid, int lign, int column, int x, int y) {
    for (int i = -grid->safezone; i < grid->safezone +1; i++) {
        for (int j = -grid->safezone; j < grid->safezone + 1; j++) {
            if (lign + i == x && column + j == y) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

int PlaceBomb(Grid* grid, int lign, int column, Coord coord) {



    int** possiblesValues = (int**)malloc((sizeof(int) * (grid->size * grid->size) * 2));
    if (possiblesValues == NULL)
        exit(1);

    int count = 0;
    int totalSize = grid->size * grid->size;

    for (int lign = 0; lign < grid->size; lign++) {
        for (int column = 0; column < grid->size; column++) {

            if (SafeZone(grid, coord.x, coord.y, lign, column) == TRUE) {
                possiblesValues[count] = (int*)malloc(sizeof(int) * 2);
                if (possiblesValues[count] == NULL)
                    exit(1);

                possiblesValues[count][0] = lign;
                possiblesValues[count][1] = column;
                count += 1;
            }
        }
    }

    int place = 0;
    int random_index;

    for (int counter = 0; counter < grid->bombCount; counter++) {

        random_index = rand() % count;

        int x = possiblesValues[random_index][0];
        int y = possiblesValues[random_index][1];

        possiblesValues[random_index][0] = possiblesValues[count - 1][0];
        possiblesValues[random_index][1] = possiblesValues[count - 1][1];

        count -= 1;

        grid->tiles[x][y].isBomb = 1;

        SetBombAround(grid, x, y);

    }


    return 1;

}

int* AskCoor(Grid* grid, int coor[2]) {

    while (TRUE) {
        printf("Choose the lign : ");
        coor[0] = GetIntInRange(0, grid->size - 1);
        printf("Choose the column : ");
        coor[1] = GetIntInRange(0, grid->size - 1);
        if (grid->tiles[coor[0]][coor[1]].revealed == 0) {
            return coor;
        }
        printf("Choose a non-revealed tile\n");
    }
    
}

BOOL UpdateGrid(Grid* grid, BOOL firststart, Coord coord) {

    int lign = coord.x;
    int column = coord.y;
    if (firststart == TRUE) {
        PlaceBomb(grid, lign, column, coord);
    }

    if (grid->tiles[lign][column].isBomb == 1 && grid->tiles[lign][column].flag == 0) {
        return TRUE;
    }

    RevealTile(grid, lign, column, firststart);

    return FALSE;
}

void InitGrid(Grid* grid) {

    grid->tiles = (Tile**)malloc(sizeof(Tile) * grid->size);
    if (grid->tiles == NULL)
        exit(1);

    for (int count = 0; count < grid->size; count++) {
        grid->tiles[count] = (Tile*)malloc(grid->size * sizeof(Tile));
        if (grid->tiles[count] == NULL)
            exit(1);

        for (int i = 0; i < grid->size; i++) {
            grid->tiles[count][i].isBomb = 0;
            grid->tiles[count][i].flag = 0;
            grid->tiles[count][i].revealed = 0;
            grid->tiles[count][i].bombAround = 0;
        }
    }
}

void PrintGrid(const Grid* grid) {


    int size = grid->size;
    printf("\t");

    for (int k = 0; k < size; k++) {
        if (k < 10) { printf("   0%d", k); }
        else { printf("   %d", k); }

    }

    printf("\n");

    for (int lign = 0; lign < size; ++lign) {

        printf(" %d \t", lign);
        for (int column = 0; column < size; ++column) {

            if (grid->tiles[lign][column].flag == 1) {
                printf(" | %sI %s", GREEN, RESET);
            }
            else if (!grid->tiles[lign][column].revealed) {
                printf(" | - ");
            }
            else if (!grid->tiles[lign][column].bombAround) {
                printf(" |   ");
            }
            else if (grid->tiles[lign][column].isBomb) {
                printf(" | %sO %s", RED, RESET);
            }
            else {
                printf(" | %s%dm%d %s", COLOR, (grid->tiles[lign][column].bombAround + 2), grid->tiles[lign][column].bombAround, RESET);
            }

        }
        printf(" | \n");
    }
}

void EndGame(Grid* grid) {

    for (int lign = 0; lign < grid->size; lign++) {
        for (int column = 0; column < grid->size; column++) {
            if (grid->tiles[lign][column].isBomb == 1) {
                grid->tiles[lign][column].flag = 0;
                grid->tiles[lign][column].revealed = 1;
            }
        }
    }
}

