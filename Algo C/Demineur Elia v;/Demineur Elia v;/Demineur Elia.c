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






SDL_Window* SetWindow(SDL_Renderer** renderer){

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

            *renderer = SDL_CreateRenderer(window, -1, 0);
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


void PrintSquare(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** surface, Tile* tile) {

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);


    SDL_Surface* text;
    SDL_Texture* texture;

    char number[2];
    number[0] = (char)tile->bombAround + '0';
    number[1] = '\0';

    text = TTF_RenderText_Solid(font, number, tile->color);

    texture = SDL_CreateTextureFromSurface(*renderer, text);


    int height, width;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { tile->rect.x + (tile->rect.w/4), tile->rect.y, width, height};

    SDL_BlitSurface(text, NULL, *surface, &newrect);

    SDL_FreeSurface(text);

}



void PrintGridSDL(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {

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
                Colors(screenSurface, 124, 124, 124, &grid->tiles[i][j].rect);

                PrintSquare(window, renderer, screenSurface, &grid->tiles[i][j]);

            }

        }
    }
    
    SDL_UpdateWindowSurface(*window);


}

void PrintTitle(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {

    TTF_Font* font = TTF_OpenFont("Pixel.ttf", 30);

    SDL_Surface* txtTitle;
    SDL_Texture* texture;

    SDL_Color white = { 255,255,255};


    txtTitle = TTF_RenderText_Solid(font, "MineSweeper", white);

    texture = SDL_CreateTextureFromSurface(*renderer, txtTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { 200 , 10, width, height };

    SDL_BlitSurface(txtTitle, NULL, *screenSurface, &newrect);

    SDL_FreeSurface(txtTitle);
}


void PrintSettings(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer, const char* title, int ypos) {

    TTF_Font* font = TTF_OpenFont("Pixel.ttf", 24);

    SDL_Surface* txtSurface;
    SDL_Texture* texture;

    SDL_Color white = { 255,255,255 };

    txtSurface = TTF_RenderText_Solid(font, title, white);

    texture = SDL_CreateTextureFromSurface(*renderer, txtSurface);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH/2 - width/2, ypos, width, height };

    SDL_BlitSurface(txtSurface, NULL, *screenSurface, &newrect);

    SDL_FreeSurface(txtSurface);

}



void MenuMineSweeper(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {

    PrintTitle(grid,screenSurface,window,renderer);

    int ypos; 
    int height, width; 

    PrintSettings(grid, screenSurface, window, renderer, "Tiles Number", 40); 

    SDL_Rect leftRectTiles = { 100 , 45, 10,10 };
    SDL_Rect rightRectTiles = { 300, 45, 10,10 };

    PrintSettings(grid, screenSurface, window, renderer, "Bombs Number", 60);

    SDL_Rect leftRectBomb = { 100, 65, 10,10 };
    SDL_Rect rightRectBomb = { 300, 65, 10,10 };


    //Square Size ; 
    //Bomb Number; 



}


void QuitGame(SDL_Window** window) {

    SDL_DestroyWindow(*window);

    TTF_Quit();
    SDL_Quit();


}


int main() {
 

    BOOL play = TRUE;

    while (play) {

        Grid myGrid;

       

        
        BOOL firststart = TRUE;
        Coord coord;

        SDL_Event event;
        BOOL round = TRUE;
        BOOL loose = FALSE;
        


        printf("Choose the size of the grid :");
        myGrid.size = GetIntInRange(10, 100);
        printf("Choose the number of mine :");

        myGrid.safezone = 1;

        int safeZoneSquare = (myGrid.safezone + 2) * (myGrid.safezone + 2);

        int maxBomb = 10 + (myGrid.size - 10) * 5;

        myGrid.bombCount = GetIntInRange(5, maxBomb);

        myGrid.remainingTiles = (myGrid.size * myGrid.size) - myGrid.bombCount;


        SDL_Renderer* myRenderer;
        SDL_Window* myWindow = SetWindow(&myRenderer);
        SDL_Surface* mySurface = SetSurface(&myWindow);


        


        srand(time(NULL));
        InitGrid(&myGrid);
        system("cls");


        MenuMineSweeper(&myGrid, &mySurface, &myWindow, &myRenderer);

        //InitGridSDL(&myGrid, &mySurface, &myWindow);


        while (round) {

            SDL_WaitEvent(&event);

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                BOOL onTile = ClickOnTile(&myGrid, event.motion.x, event.motion.y, &coord);
                if (onTile) {

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        loose = UpdateGrid(&myGrid, firststart, coord);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow, &myRenderer);
                        firststart = FALSE;
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT) {
                        PlaceFlag(&myGrid, coord);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow,&myRenderer);

                    }

                    if (loose) {
                        printf("\n");
                        printf("You Loose ! ");
                        round = FALSE;
                        EndGame(&myGrid);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow,&myRenderer);

                    }

                    if (CheckWin(&myGrid)) {
                        printf("\n");
                        printf("You Win ! ");
                        round = FALSE;
                        PrintGridSDL(&myGrid, &mySurface, &myWindow,&myRenderer);
                    }
                }
            }

        }


        QuitGame(&myWindow);


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