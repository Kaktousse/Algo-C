#pragma once
#include <SDL.h>


typedef int BOOL;

#define TRUE 1
#define FALSE 0

#define COLOR "\x1B[3"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m" 
#define RESET "\x1B[0m"



typedef struct Tile
{
    int isBomb;
    int bombAround;
    int revealed;
    int flag;

    SDL_Rect rect;

} Tile;

typedef struct Grid
{
    Tile** tiles;
    int size;
    int bombCount;
    int flag;
    int safezone;

    int remainingTiles;

} Grid;

int CheckWin(const Grid* grid);

int PlaceFlag(Grid* grid);

void SetBombAround(Grid* grid, int setlign, int setcolumn);

Tile* GetTile(Grid* grid, int x, int y);

void RevealTile(Grid* grid, int setlign, int setcolumn, BOOL firststart);

BOOL SafeZone(Grid* grid, int lign, int column, int x, int y);

int PlaceBomb(Grid* grid, int lign, int column, int playCoor[2]);

int* AskCoor(Grid* grid, int coor[2]);

int UpdateGrid(Grid* grid, BOOL firststart, int coor[2]);

void InitGrid(Grid* grid);

void PrintGrid(const Grid* grid);

void EndGame(Grid* grid);