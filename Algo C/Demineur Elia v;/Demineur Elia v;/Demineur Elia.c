#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 
#include <conio.h>
#include <windows.h>
#include <SDL.h>

#include "grid.h"
#include "getinput.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400




void InitGridSDL(Grid* grid, SDL_Surface* screensurface) {




    int rectWidth, rectHeight; 
    rectWidth = (SCREEN_WIDTH / grid->size) -10 ;
    rectHeight = (SCREEN_HEIGHT / grid->size) -10;

    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {

            grid->tiles[i][j].rect.w = rectWidth;
            grid->tiles[i][j].rect.h = rectHeight;

            grid->tiles[i][j].rect.x = (j * (rectHeight *1.25)) +15;
            grid->tiles[i][j].rect.y = (i * (rectWidth *1.25)) +15;


            if (SDL_FillRect(screensurface, &grid->tiles[i][j].rect, SDL_MapRGB(screensurface->format, 57, 57, 57)) != 0) {
                printf("HEEEEELP");
            }

        }
    }
    


}





void SetWindow(Grid* grid, SDL_Window* window, SDL_Surface* screenSurface){

     

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    else
    {
        //Create window

        window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        

        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }

        

        else
        {

            screenSurface = SDL_GetWindowSurface(window);
           
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 218, 218, 218));

            //Update the surface
            SDL_UpdateWindowSurface(window);
        }
    }



}






void QuitGame(SDL_Window* window) {

    SDL_DestroyWindow(window);


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

        SDL_Window* window = NULL;

        SDL_Surface* screenSurface = NULL;

        

        myGrid.remainingTiles = (myGrid.size * myGrid.size) - myGrid.bombCount;
        
        BOOL round = TRUE;
        BOOL firststart = TRUE;
        BOOL loose = TRUE;
        int coor[2];

        srand(time(NULL));
        InitGrid(&myGrid);
        system("cls");

        SetWindow(&myGrid, window, screenSurface);

        InitGridSDL(&myGrid, screenSurface);



        while (round) {

            printf("\nPress F to place a flag or Choose the tile to reveal.\n ");


            if (_kbhit) {
                if (_getch() == 'f') {
                    PlaceFlag(&myGrid);
                }
                else {
                    AskCoor(&myGrid, coor);
                    loose = UpdateGrid(&myGrid, firststart,coor);
                    firststart = FALSE;

                    if (CheckWin(&myGrid)) {
                        printf("\n");
                        printf("You Win ! ");
                        round = FALSE;
                    }
                    if (loose) {
                        printf("\n");
                        printf("You Loose ! ");
                        round = FALSE;
                        EndGame(&myGrid);
                    }
                }
            }
        }


        printf("\n");
        PrintGrid(&myGrid);
        printf("Do you want to restart ? \n");

        if (!YesOrNo()) {
            play = FALSE;
        }

        for (int i = 0; i < myGrid.size; i++) {
            free(myGrid.tiles[i]);
        }
        free(myGrid.tiles);

        QuitGame(window);
    }

    
    return 0;
}