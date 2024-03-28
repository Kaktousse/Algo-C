#include <stdlib.h> 
#include <time.h> 
#include <stdio.h> 
#include <conio.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "grid.h"
#include "getinput.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1000


typedef struct Window
{
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
};


void Colors(Window* pWindow, int blue, int green, int red, const SDL_Rect* rect) {

    SDL_SetRenderDrawColor(pWindow->pRenderer, blue, green, red, 255);
    SDL_RenderFillRect(pWindow->pRenderer, rect);
}



void QuitGame(SDL_Window** window) {

    SDL_DestroyWindow(*window);

    TTF_Quit();
    SDL_Quit();


}

void PrintFlags(Grid* grid, Window* pWindow) {


    TTF_Font* font = TTF_OpenFont("pixel.ttf", 20);

    SDL_Surface* textFlagTitle;
    SDL_Texture* textureFlagTitle;
    SDL_Color titleColor = { 0,0,0 };

    textFlagTitle = TTF_RenderText_Solid(font, "Remaining Flags", titleColor);

    textureFlagTitle = SDL_CreateTextureFromSurface(pWindow->pRenderer, textFlagTitle);


    int height, width;

    SDL_QueryTexture(textureFlagTitle, NULL, NULL, &width, &height);

    SDL_Rect flagTitleRect = { 10 , 10, width, height };

    SDL_FreeSurface(textFlagTitle);
    SDL_DestroyTexture(textureFlagTitle);


    TTF_Font* flagfont = TTF_OpenFont("pixel.ttf", 20);

    SDL_Surface* textFlag;
    SDL_Texture* textureFlag;

    char flags[5];
    flags[0] = (char)(int)(grid->flag / 1000) + '0';
    flags[1] = (char)((int)(grid->flag / 100) - ((int)(grid->flag / 1000) * 10)) + '0';
    flags[2] = (char)((int)(grid->flag / 10) - ((int)(grid->flag / 100)) * 10)  + '0';
    flags[3] = (char)((int)grid->flag -  ((int)(grid->flag / 10) * 10) ) + '0';
    flags[4] = '\0';


    textFlag = TTF_RenderText_Solid(flagfont, flags , titleColor);

    textureFlag = SDL_CreateTextureFromSurface(pWindow->pRenderer, textFlag);

    SDL_QueryTexture(textureFlag, NULL, NULL, &width, &height);

    SDL_Rect flagRect = { 10, 50, width, height };

    SDL_RenderCopy(pWindow->pRenderer, textureFlag, NULL, &flagRect);

    SDL_FreeSurface(textFlag);
    SDL_DestroyTexture(textureFlag);


}

void InitGridSDL(Grid* grid, Window* pWindow) {


    Colors(pWindow, 218, 218, 218, NULL);

    TTF_Font* font = TTF_OpenFont("pixel.ttf", 70);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 0,0,0 };

    textTitle = TTF_RenderText_Solid(font, "MineSweeper", titleColor);

    texture = SDL_CreateTextureFromSurface(pWindow->pRenderer, textTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);

    SDL_FreeSurface(textTitle);
    SDL_DestroyTexture(texture);

    int rectWidth, rectHeight;
    rectWidth = (800 / grid->size);
    rectHeight = rectWidth;

    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {

            grid->tiles[i][j].rect.w = rectWidth;
            grid->tiles[i][j].rect.h = rectHeight;

            grid->tiles[i][j].rect.x = (j * (rectHeight * 1.10) + 10) ;
            grid->tiles[i][j].rect.y = (i * (rectWidth *1.10)) +120;

            Colors(pWindow, 84, 84, 84, &grid->tiles[i][j].rect);
              
        }
    }

    SDL_RenderPresent(pWindow->pRenderer);
}




/*

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
*/
/*
SDL_Surface* SetSurface(SDL_Window** window) {

    SDL_Surface* screenSurface = NULL;

    screenSurface = SDL_GetWindowSurface(*window);
    return screenSurface;
}
*/

BOOL BoxCollider(SDL_Rect* rect, int clickX, int clickY) {
    if ( (clickX >= rect->x && clickX <= rect->x + rect->w) && (clickY >= rect->y && clickY <= rect->y + rect->h) ) {
        return TRUE;
    }

    return FALSE;
}


BOOL ClickOnTile(Grid* grid, int clickX, int clickY, Coord* coord) {


    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {
            if (BoxCollider( &grid->tiles[i][j].rect, clickX, clickY)) {
                coord->x = i; 
                coord->y = j;
                return TRUE;
            }
        }
    }
   
    return FALSE;
}


void PrintSquare(Window* pWindow, Tile* tile, Grid* grid, TTF_Font* font) {


    SDL_Surface* text;
    SDL_Texture* texture;

    char number[2];
    number[0] = (char)tile->bombAround + '0';
    number[1] = '\0';

    text = TTF_RenderText_Solid(font, number, tile->color);

    texture = SDL_CreateTextureFromSurface(pWindow->pRenderer, text);


    int height = tile->rect.h , width = tile->rect.w;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { tile->rect.x + (tile->rect.w/4), tile->rect.y+(tile->rect.h / 8), width, height};
 
    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);


    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);

}



void PrintGridSDL(Grid* grid, Window* pWindow, TTF_Font* font) {


    Colors(pWindow, 218, 218, 218, NULL);

    TTF_Font* titleFont = TTF_OpenFont("pixel.ttf", 70);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 0,0,0 };

    textTitle = TTF_RenderText_Solid(titleFont, "MineSweeper", titleColor);

    texture = SDL_CreateTextureFromSurface(pWindow->pRenderer, textTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);

    SDL_FreeSurface(textTitle);
    SDL_DestroyTexture(texture);





    for (int i = 0; i < grid->size; i++) {
        for (int j = 0; j < grid->size; j++) {

            if (grid->tiles[i][j].flag == 1) {
                Colors(pWindow, 14, 185, 0, &grid->tiles[i][j].rect);
            }
            else if (!grid->tiles[i][j].revealed) {
                Colors(pWindow, 84, 84, 84, &grid->tiles[i][j].rect);
            }
            else if (!grid->tiles[i][j].bombAround) {
                Colors(pWindow, 180,180,180, &grid->tiles[i][j].rect);
            }
            else if (grid->tiles[i][j].isBomb) {
                Colors(pWindow, 142, 0, 0, &grid->tiles[i][j].rect);
            }
            else {
                Colors(pWindow, 124, 124, 124, &grid->tiles[i][j].rect);

                PrintSquare(pWindow, &grid->tiles[i][j], grid, font);

            }

        }
    }
    
    PrintFlags(grid, pWindow);

    SDL_RenderPresent(pWindow->pRenderer);
}

void PrintTitle(Grid* grid, Window* pWindow, const char* title) {

    TTF_Font* font = TTF_OpenFont("pixel.ttf", 80);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 255,255,255 };

    textTitle = TTF_RenderText_Solid(font, title, titleColor);

    texture = SDL_CreateTextureFromSurface(pWindow->pRenderer, textTitle);

    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 30, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);

    SDL_FreeSurface(textTitle);
    SDL_DestroyTexture(texture);
}


void PrintSettings(Grid* grid, Window* pWindow, const char* title, int xpos, int ypos, int size) {

    TTF_Font* font = TTF_OpenFont("pixel.ttf", size);

    SDL_Surface* txtSurface;
    SDL_Texture* texture;

    SDL_Color white = { 255,255,255 };

    txtSurface = TTF_RenderText_Solid(font, title, white);

    texture = SDL_CreateTextureFromSurface(pWindow->pRenderer, txtSurface);



    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2 + xpos, ypos, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);


    SDL_FreeSurface(txtSurface);
    SDL_DestroyTexture(texture);

}

void PrintMenu(Grid* grid, Window* pWindow, char* numberTile, char* numberBomb, SDL_Rect* confirm) {

    PrintTitle(grid, pWindow, "MineSweeper");

    int ypos;
    int height, width;

    PrintSettings(grid, pWindow, "Tiles Number", 0, SCREEN_HEIGHT / 4, 50);

    PrintSettings(grid, pWindow, "<", -80, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, pWindow, "<<", -150, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, pWindow, numberTile, 0, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, pWindow, ">", 80, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, pWindow, ">>", 150, SCREEN_HEIGHT / 4 + 80, 50);


    PrintSettings(grid, pWindow, "Bombs Number", 0, SCREEN_HEIGHT / 2, 50);


    PrintSettings(grid, pWindow, "<", -90, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, "<<", -160, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, "<<<", -260, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, numberBomb, 0, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, ">", 90, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, ">>", 160, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, pWindow, ">>>", 260, SCREEN_HEIGHT / 2 + 80, 50);


    Colors(pWindow, 50, 50, 50, confirm);

    PrintSettings(grid, pWindow, "Confirm", 0, (SCREEN_HEIGHT / 4) * 3 + 20, 50);
}





void MenuMineSweeper(Grid* grid, Window* pWindow) {



    SDL_Event eventMenu; 
    BOOL clickConfirm = FALSE; 

    int tileCount = 10; 
    int bombCount = 5 ; 


    char* numberTile = (char*)malloc(sizeof(char) * 3);
    if (numberTile == NULL) {
        exit(1);
    }
    char* numberBombs = (char*)malloc(sizeof(char) * 2);
    if (numberBombs == NULL) {
        exit(1);
    }

    int tileSize = 3; 
    int bombSize = 2; 

    while (!clickConfirm) {


        int maxBomb = (tileCount*tileCount) - grid->safezone ;

        if (bombCount > maxBomb) {
            bombCount = maxBomb;
        }

        if (tileCount <= 99) {
            if (tileSize == 4) {
                tileSize = 3;
                numberTile = (char*)realloc(numberTile, sizeof(char) * 3);
                if (numberTile == NULL) {
                    exit(1);
                }

            }

            numberTile[0] = (char)((int)(tileCount / 10)) + '0';
            numberTile[1] = (char)( tileCount - ( (int)(tileCount / 10) * 10) ) + '0';
            numberTile[2] = '\0';
        }
        else {
            if (tileSize == 3) {
                tileSize = 4;
                numberTile = (char*)realloc(numberTile, sizeof(char) * 4);
                if (numberTile == NULL) {
                    exit(1);
                }
            }
            numberTile[0] = '1';
            numberTile[1] = '0';
            numberTile[2] = '0';
            numberTile[3] = '\0';
        }




        if (bombCount <= 9) {
            if (bombSize != 2) {
                bombSize = 2;
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 2);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }
            numberBombs[0] = (char)bombCount + '0';
            numberBombs[1] = '\0';
        }
        else if (bombCount <= 99) {
            if (bombSize != 3 ) {
                bombSize = 3; 
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 3);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }
            numberBombs[0] = (char)(int)(bombCount / 10) + '0';
            numberBombs[1] = (char)(bombCount - ((int)(bombCount / 10) * 10)) + '0';
            numberBombs[2] = '\0';
        }
        else if (bombCount <= 999){
            if (bombSize != 4) {
                bombSize = 4; 
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 4);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }

            numberBombs[0] = (char) (int)(bombCount / 100 ) +'0';
            numberBombs[1] = (char) ((int)(bombCount / 10)  - ( (int)(bombCount / 100) *10 ) ) + '0';
            numberBombs[2] = (char)(bombCount - (int)(bombCount / 10) * 10 )+ '0';
            numberBombs[3] = '\0';
        }
        else{
            if (bombSize != 5) {
                bombSize = 5;
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 5);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }
            numberBombs[0] = (char)((int)(bombCount / 1000)) + '0';
            numberBombs[1] = (char)((int)(bombCount / 100) - ((int)(bombCount / 1000)) * 10) + '0';
            numberBombs[2] = (char)((int)(bombCount /10) - (int)(bombCount / 100)*10)   + '0';
            numberBombs[3] = (char)(bombCount - (int)(bombCount / 10) * 10) + '0';
            numberBombs[4] = '\0';
        }



        Colors(pWindow, 0, 0, 0, NULL);

        SDL_Rect leftRectTiles = { 355, SCREEN_HEIGHT / 4 + 80, 30,50 };
        SDL_Rect rightRectTiles = { 515, SCREEN_HEIGHT / 4 + 80, 30,50 };

        SDL_Rect doubleLeftRectTiles = { 275, SCREEN_HEIGHT / 4 + 80, 50,50 };
        SDL_Rect doubleRightRectTiles = { 575, SCREEN_HEIGHT / 4 + 80, 50,50 };


        SDL_Rect leftRectBombs = { 345, SCREEN_HEIGHT / 2 + 80, 30,50 };
        SDL_Rect rightRectBombs = { 525, SCREEN_HEIGHT / 2 + 80, 30,50 };


        SDL_Rect doubleLeftRectBombs = { 265, SCREEN_HEIGHT / 2 + 80, 50,50 };
        SDL_Rect doubleRightRectBombs = { 585, SCREEN_HEIGHT / 2+ 80, 50,50 };


        SDL_Rect tripleLeftRectBombs = { 150, SCREEN_HEIGHT / 2 + 80, 80,50 };
        SDL_Rect tripleRightRectBombs = { 670, SCREEN_HEIGHT / 2 + 80, 80,50 };


        SDL_Rect confirm = { 300 , (SCREEN_HEIGHT / 4) * 3, 300,100 };

        PrintMenu(grid, pWindow, numberTile, numberBombs, &confirm);

        SDL_SetRenderDrawColor(pWindow->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pWindow->pRenderer);
        SDL_RenderPresent(pWindow->pRenderer);

        SDL_WaitEvent(&eventMenu);
        if (eventMenu.type == SDL_MOUSEBUTTONDOWN ) {
            if (eventMenu.button.button == SDL_BUTTON_LEFT) {

                if (BoxCollider(&leftRectTiles, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (tileCount > 10) {
                        tileCount -= 1; 
                    }
                }

                if (BoxCollider(&rightRectTiles, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (tileCount < 100) {
                        tileCount += 1;
                    }
                }

                if (BoxCollider(&doubleLeftRectTiles, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (tileCount >= 20) {
                        tileCount -= 10;
                    }
                }

                if (BoxCollider(&doubleRightRectTiles, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (tileCount <= 90) {
                        tileCount += 10;
                    }
                }



                if (BoxCollider(&leftRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount > 5) {
                        bombCount -= 1;
                    }
                }
                if (BoxCollider(&rightRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount < maxBomb ) {
                        bombCount += 1;
                    }
                }
                if (BoxCollider(&doubleLeftRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount >= 15) {
                        bombCount -= 10;
                    }
                }

                if (BoxCollider(&doubleRightRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount <= maxBomb-10) {
                        bombCount += 10;
                    }
                }
                if (BoxCollider(&tripleLeftRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount >= 105) {
                        bombCount -= 100;
                    }
                }

                if (BoxCollider(&tripleRightRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount <= maxBomb-100) {
                        bombCount += 100;
                    }
                }

                if ( BoxCollider(&confirm, eventMenu.motion.x, eventMenu.motion.y)) {
                    clickConfirm = TRUE;
                }
            }
        }


    }

    free(numberBombs);
    free(numberTile);

    grid->size = tileCount; 
    grid->bombCount = bombCount; 

}

BOOL GameOverScreen(Grid* grid, SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screenSurface, BOOL win) {
    SDL_Event eventEnd;

    if (win) { // Victoire  
        Colors(screenSurface, 0, 0, 0, NULL);

        PrintTitle(grid, window, renderer, screenSurface, "YOU WIN !!!");



    }
    else { //Defaite
        Colors(screenSurface, 0, 0, 0, NULL);

        PrintTitle(grid, window, renderer, screenSurface, "YOU LOOSE !!!");
    
    }

    SDL_Rect retry = { 225 , (SCREEN_HEIGHT / 4) * 3, 150,100 };

    SDL_Rect quit = { 525 , (SCREEN_HEIGHT / 4) * 3, 150,100 };

    PrintSettings(grid, window, renderer, screenSurface, "Retry", -150, (SCREEN_HEIGHT / 4) * 3 + 20, 50);

    PrintSettings(grid, window, renderer, screenSurface, "QUIT", 150, (SCREEN_HEIGHT / 4) * 3 + 20, 50);

    SDL_UpdateWindowSurface(*window);

    while (TRUE) {
        SDL_WaitEvent(&eventEnd);
        if (eventEnd.type == SDL_MOUSEBUTTONDOWN && eventEnd.button.button == SDL_BUTTON_LEFT) {

            if (BoxCollider(&retry, eventEnd.motion.x, eventEnd.motion.y)) {
                QuitGame(window);
                return TRUE;
            }
            if (BoxCollider(&quit, eventEnd.motion.x, eventEnd.motion.y)) {
                QuitGame(window);
                return FALSE;
            }
        }
    }
}


void CreateSurface(Grid* grid) {

}

void CreateTexture(Grid* grid) {

}


void InitSDL(Window* pSDLWindow)
{
    //Etape 1: Initialisation de la SDL
    int iError = SDL_Init(SDL_INIT_VIDEO);
    if (iError != 0)
    {
        exit(1);
    }

    TTF_Init(); //#TODO check if success

    //Etape 2: Construction de la Fenêtre (Window)
    pSDLWindow->pWindow = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (pSDLWindow->pWindow == NULL)
    {
        exit(1);
    }

    //Etape 3: Construction du Canvas de dessin (Renderer)
    pSDLWindow->pRenderer = SDL_CreateRenderer(pSDLWindow->pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (pSDLWindow->pRenderer == NULL)
    {
        exit(1);
    }

}

void SDLQuit(Window* pSDLWindow)
{
    //Etape 1: Destruction du Canvas (Renderer)
    SDL_DestroyRenderer(pSDLWindow->pRenderer);

    //Etape 2: Destruction de la Fenêtre (Window)
    SDL_DestroyWindow(pSDLWindow->pWindow);

    //Etape 3: Fermeture de la SDL
    SDL_Quit();
}


int main() {
 
    //INIT
    Window oSDLWindow;
    InitSDL(&oSDLWindow);




    BOOL play = TRUE;

    while (play) {

        system("cls");


        Grid myGrid;

        
        BOOL firststart = TRUE;
        Coord coord;

        SDL_Event event;
        BOOL round = TRUE;
        BOOL loose = FALSE;

        BOOL winOrLoose; 
        
        //SDL_Renderer* myRenderer = NULL;
        //SDL_Window* myWindow = SetWindow(&myRenderer);

        //SDL_Surface* mySurface = SetSurface(&myWindow);


        system("cls");


        /* RESOURCES */

        CreateSurface(&myGrid);
        CreateTexture(&myGrid);


        int safezone = 1;

        myGrid.safezone = (safezone + 2) * (safezone + 2);
 
        MenuMineSweeper(&myGrid, &oSDLWindow);


        TTF_Font* squareFont = TTF_OpenFont("arial.ttf", (int)55 / (myGrid.size / 10));

        myGrid.remainingTiles = (myGrid.size * myGrid.size) - myGrid.bombCount;

        printf("%d %d", myGrid.size, myGrid.bombCount);
        srand(time(NULL));
        InitGrid(&myGrid);
     
        myGrid.flag = myGrid.bombCount;

        InitGridSDL(&myGrid, &oSDLWindow);



        while (round) {

            SDL_WaitEvent(&event);

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                BOOL onTile = ClickOnTile(&myGrid, event.motion.x, event.motion.y, &coord);
                if (onTile) {

                    if (event.button.button == SDL_BUTTON_LEFT) {
                        loose = UpdateGrid(&myGrid, firststart, coord);
                        PrintGridSDL(&myGrid, &oSDLWindow, squareFont);
                        firststart = FALSE;
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT) {
                        PlaceFlag(&myGrid, coord);
                        PrintGridSDL(&myGrid, &oSDLWindow, squareFont);

                    }

                    if (loose) {
                        winOrLoose = FALSE; 
                        round = FALSE;
                        EndGame(&myGrid);
                        PrintGridSDL(&myGrid, &mySurface, &myWindow,&myRenderer, squareFont);
                        SDL_Delay(1500);


                    }

                    if (CheckWin(&myGrid)) {
                        winOrLoose = TRUE;
                        round = FALSE;
                        PrintGridSDL(&myGrid, &mySurface, &myWindow,&myRenderer, squareFont);
                        SDL_Delay(1500);
                    }
                }
            }
        }



        if (!GameOverScreen(&myGrid, &myWindow, &myRenderer, &mySurface, winOrLoose)) {
            play = FALSE;
        }

        for (int i = 0; i < myGrid.size; i++) {
            free(myGrid.tiles[i]);
        }
        free(myGrid.tiles);

    }

        
    
    return 0;
}