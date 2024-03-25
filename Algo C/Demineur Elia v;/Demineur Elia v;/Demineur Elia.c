#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 
#include <conio.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "grid.h"
#include "getinput.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400



void Colors(SDL_Surface** screenSurface, int blue, int green, int red, const SDL_Rect* rect) {


    SDL_FillRect(*screenSurface, rect, SDL_MapRGB((*screenSurface)->format, blue, green, red));


}



void InitGridSDL(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window) {

    int rectWidth, rectHeight; 
    rectWidth = (SCREEN_WIDTH / grid->size) -10 ;
    rectHeight = (SCREEN_HEIGHT / grid->size) -10;

    Colors(screenSurface, 218, 218, 218, NULL);

    

    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {

            grid->tiles[i][j].rect.w = rectWidth;
            grid->tiles[i][j].rect.h = rectHeight;

            grid->tiles[i][j].rect.x = (j * (rectHeight * 1.25)) +15;
            grid->tiles[i][j].rect.y = (i * (rectWidth *1.25)) +15;

            Colors(screenSurface, 84, 84, 84, &grid->tiles[i][j].rect);
              
        }
    }
    
    SDL_UpdateWindowSurface(*window);


}






SDL_Window* SetWindow(){

    SDL_Window* window = NULL;
    

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    else
    {
        TTF_Init();
        //Create window
        window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }

        else
        {
            return window;
        }
    }


}

SDL_Surface* SetSurface(SDL_Window** window) {

    SDL_Surface* screenSurface = NULL;

    screenSurface = SDL_GetWindowSurface(*window);
    return screenSurface;
}


BOOL BoxCollider(int boxX, int boxY,int boxSize, int clickX, int clickY) {
    if ( (clickX >= boxX && clickX <= boxX + boxSize) && (clickY >= boxY && clickY <= boxY + boxSize) ) {
        return TRUE;
    }

    return FALSE;
}


BOOL ClickOnTile(Grid* grid, int clickX, int clickY, Coord* coord) {


    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {
            if (BoxCollider(grid->tiles[i][j].rect.x, grid->tiles[i][j].rect.y, grid->tiles[i][j].rect.w, clickX, clickY)) {
                coord->x = i; 
                coord->y = j;
                return TRUE;
            }
        }
    }
   
    return FALSE;
}


void PrintGridSDL(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Texture** texture, SDL_Renderer** renderer) {

    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {

            if (grid->tiles[i][j].flag == 1) {
                Colors(screenSurface, 14, 185, 0, &grid->tiles[i][j].rect);
            }
            else if (!grid->tiles[i][j].revealed) {
                Colors(screenSurface, 84, 84, 84, &grid->tiles[i][j].rect);
            }
            else if (!grid->tiles[i][j].bombAround) {
                Colors(screenSurface, 180,180,180, &grid->tiles[i][j].rect);
            }
            else if (grid->tiles[i][j].isBomb) {
                Colors(screenSurface, 142, 0, 0, &grid->tiles[i][j].rect);
            }
            else {
                SDL_RenderCopy(*renderer, *texture, &grid->tiles[i][j].rect, &grid->tiles[i][j].rect);
                SDL_RenderPresent(*renderer);
            }

        }
    }

    SDL_UpdateWindowSurface(*window);


}

void QuitGame(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, SDL_Surface** surface) {


    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_DestroyTexture(*texture);
    SDL_FreeSurface(*surface);
    TTF_Quit();
    SDL_Quit();


}


int main() {
 

    BOOL play = TRUE;

    while (play) {

        Grid myGrid;

        printf("Choose the size of the grid :");
        myGrid.size = GetIntInRange(10, 100);
        printf("Choose the number of mine :");

        myGrid.safezone = 1;

        int safeZoneSquare = (myGrid.safezone + 2) * (myGrid.safezone + 2);

        int maxBomb = 10 + (myGrid.size-10)*5 ;

        myGrid.bombCount = GetIntInRange(5, maxBomb)  ;

        myGrid.remainingTiles = (myGrid.size * myGrid.size) - myGrid.bombCount;
        


        srand(time(NULL));
        InitGrid(&myGrid);
        system("cls");

        SDL_Window* myWindow = SetWindow();
        SDL_Surface* mySurface = SetSurface(&myWindow);
        InitGridSDL(&myGrid, &mySurface, &myWindow);

       


        BOOL firststart = TRUE;
        Coord coord;

        SDL_Event event;
        BOOL round = TRUE;
        BOOL loose = FALSE;
        SDL_Renderer* renderer = SDL_CreateRenderer(myWindow, -1, 0);

        TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

        SDL_Color color = { 255,0,0 };
        SDL_Surface* txtsurface = TTF_RenderText_Solid(font, "TAMERE", color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, txtsurface);

        while (round) {

            SDL_WaitEvent(&event);

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                BOOL onTile = ClickOnTile(&myGrid, event.motion.x, event.motion.y, &coord);
                if (onTile) {

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        loose = UpdateGrid(&myGrid, firststart, coord);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow, &texture, &renderer);
                        firststart = FALSE;
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT){
                        PlaceFlag(&myGrid, coord);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow, &texture, &renderer);

                    }

                    if (loose) {
                        printf("\n");
                        printf("You Loose ! ");
                        round = FALSE;
                        EndGame(&myGrid);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow, &texture, &renderer);

                    }

                    if (CheckWin(&myGrid)) {
                        printf("\n");
                        printf("You Win ! ");
                        round = FALSE;
                        PrintGridSDL(&myGrid, &mySurface, &myWindow, &texture, &renderer);
                    }
                }
            }
               
        }


        QuitGame(&myWindow, &renderer, &texture, &txtsurface);

        printf("\n");
        printf("Do you want to restart ? \n");

        if (!YesOrNo()) {
            play = FALSE;
        }

        for (int i = 0; i < myGrid.size; i++) {
            free(myGrid.tiles[i]);
        }
        free(myGrid.tiles);

    }

    
    return 0;
}