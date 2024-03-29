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

    TTF_Font* fontFlag;
    TTF_Font* fontTitle;
    TTF_Font* fontNumbers; 
    TTF_Font* fontSettings;


    SDL_Surface* surfaceWin;
    SDL_Texture* textureWin;

    SDL_Surface* surfaceLoose;
    SDL_Texture* textureLoose;

    SDL_Surface* surfaceFlagTitle;
    SDL_Texture* textureFlagTitle;

    SDL_Surface* surfaceNumbers[8]; 
    SDL_Texture* textureNumbers[8];

    SDL_Surface* surfaceTitle;
    SDL_Texture* textureTitle;

    SDL_Surface* surfaceMenuTitle;
    SDL_Texture* textureMenuTitle; 

    SDL_Surface* surfaceSettings[11];
    SDL_Texture* textureSettings[11];


} Window ;




void SDLQuit(Window* pSDLWindow)
{
    //Etape 1: Destruction du Canvas (Renderer)
    SDL_DestroyRenderer(pSDLWindow->pRenderer);

    //Etape 2: Destruction de la Fenêtre (Window)
    SDL_DestroyWindow(pSDLWindow->pWindow);

    //Etape 3: Fermeture de la SDL
    SDL_Quit();
}




void Colors(Window* pWindow, int blue, int green, int red, const SDL_Rect* rect) {

    SDL_SetRenderDrawColor(pWindow->pRenderer, blue, green, red, 255);
    SDL_RenderFillRect(pWindow->pRenderer, rect);
}


void PrintFlags(Grid* grid, Window* pWindow) {


    int width, height;

    SDL_QueryTexture(pWindow->textureFlagTitle, NULL, NULL, &width, &height);

    SDL_Rect flagTitleRect = { 10 , 15, width, height };

    SDL_RenderCopy(pWindow->pRenderer, pWindow->textureFlagTitle, NULL, &flagTitleRect);


    char flags[5];
    flags[0] = (char)(int)(grid->flag / 1000) + '0';
    flags[1] = (char)((int)(grid->flag / 100) - ((int)(grid->flag / 1000) * 10)) + '0';
    flags[2] = (char)((int)(grid->flag / 10) - ((int)(grid->flag / 100)) * 10) + '0';
    flags[3] = (char)((int)grid->flag - ((int)(grid->flag / 10) * 10)) + '0';
    flags[4] = '\0';

    SDL_Color black = { 0,0,0 };
    


    SDL_Surface* surfaceFlagNumbers = TTF_RenderText_Solid(pWindow->fontFlag, flags, black);

    SDL_Texture* textureFlagNumbers = SDL_CreateTextureFromSurface(pWindow->pRenderer, surfaceFlagNumbers);



    SDL_QueryTexture(textureFlagNumbers, NULL, NULL, &width, &height);

    SDL_Rect flagRect = { 10, 50, width, height };

    SDL_RenderCopy(pWindow->pRenderer, textureFlagNumbers, NULL, &flagRect);

    SDL_FreeSurface(surfaceFlagNumbers);
    SDL_DestroyTexture(textureFlagNumbers);


}


void InitGridSDL(Grid* grid, Window* pWindow) {


    SDL_SetRenderDrawColor(pWindow->pRenderer, 218, 218, 218, 255);
    SDL_RenderClear(pWindow->pRenderer);

    int height, width;

    SDL_QueryTexture(pWindow->textureTitle, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_RenderCopy(pWindow->pRenderer, pWindow->textureTitle, NULL, &newrect);


    PrintFlags(grid, pWindow);

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


void PrintSquare(Window* pWindow, Tile* tile, Grid* grid) {

    int height = tile->rect.h , width = tile->rect.w;
    SDL_QueryTexture(pWindow->textureNumbers[tile->bombAround-1], NULL, NULL, &width, &height);

    SDL_Rect newrect = { tile->rect.x + (tile->rect.w/4), tile->rect.y+(tile->rect.h / 8), width, height};
 
    SDL_RenderCopy(pWindow->pRenderer, pWindow->textureNumbers[tile->bombAround - 1], NULL, &newrect);

}



void PrintGridSDL(Grid* grid, Window* pWindow) {



    SDL_SetRenderDrawColor(pWindow->pRenderer, 218, 218, 218, 255);

    SDL_RenderClear(pWindow->pRenderer);

    

    int width, height; 
    SDL_QueryTexture(pWindow->textureTitle, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_RenderCopy(pWindow->pRenderer, pWindow->textureTitle, NULL, &newrect);

    PrintFlags(grid, pWindow);

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

                PrintSquare(pWindow, &grid->tiles[i][j], grid);

            }

        }
    }
    
    

    SDL_RenderPresent(pWindow->pRenderer);
}

void PrintTitle(Grid* grid, Window* pWindow, SDL_Texture* texture) {

 

    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 30, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);
}


void PrintSettings(Grid* grid,Window* pWindow,  SDL_Texture* texture,int xpos, int ypos) {

    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2 + xpos, ypos, width, height };

    SDL_RenderCopy(pWindow->pRenderer, texture, NULL, &newrect);

}

void PrintUndefinedSettings(Grid* grid, Window* pWindow, char* text, int xpos, int ypos) {


    SDL_Color white = { 255,255,255 };

    SDL_Surface* temporarySurface = TTF_RenderText_Solid(pWindow->fontSettings, text, white);

    SDL_Texture* temporaryTexture = SDL_CreateTextureFromSurface(pWindow->pRenderer, temporarySurface);

    int width, height;

    SDL_QueryTexture(temporaryTexture, NULL, NULL, &width, &height);

    SDL_Rect rectSettings = { SCREEN_WIDTH / 2 - width / 2 + xpos, ypos, width, height };

    SDL_RenderCopy(pWindow->pRenderer, temporaryTexture, NULL, &rectSettings);

    SDL_FreeSurface(temporarySurface);
    SDL_DestroyTexture(temporaryTexture);


}


void PrintMenu(Grid* grid, Window* pWindow, char* numberTile, char* numberBomb, SDL_Rect* confirm) {

    PrintTitle(grid, pWindow, pWindow->textureMenuTitle);

    int ypos;
    int height, width;

    PrintSettings(grid, pWindow, pWindow->textureSettings[6],0, SCREEN_HEIGHT / 4);

    PrintSettings(grid, pWindow, pWindow->textureSettings[0], -80, SCREEN_HEIGHT / 4 + 80);

    PrintSettings(grid, pWindow, pWindow->textureSettings[1], -150, SCREEN_HEIGHT / 4 + 80 );

    PrintUndefinedSettings(grid, pWindow, numberTile, 0, SCREEN_HEIGHT / 4 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[3], 80, SCREEN_HEIGHT / 4 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[4], 150, SCREEN_HEIGHT / 4 + 80 );


    PrintSettings(grid, pWindow, pWindow->textureSettings[7], 0, SCREEN_HEIGHT / 2 );


    PrintSettings(grid, pWindow, pWindow->textureSettings[0], -90, SCREEN_HEIGHT / 2 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[1], -160, SCREEN_HEIGHT / 2 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[2], -260, SCREEN_HEIGHT / 2 + 80 );

    PrintUndefinedSettings(grid, pWindow, numberBomb, 0, SCREEN_HEIGHT / 2 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[3], 90, SCREEN_HEIGHT / 2 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[4], 160, SCREEN_HEIGHT / 2 + 80 );

    PrintSettings(grid, pWindow, pWindow->textureSettings[5], 260, SCREEN_HEIGHT / 2 + 80 );


    Colors(pWindow, 50, 50, 50, confirm);

    PrintSettings(grid, pWindow, pWindow->textureSettings[8], 0, (SCREEN_HEIGHT / 4) * 3 + 20);
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



        //'x''\0'
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
        //'x''x''\0'
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
        //'x''x''x''\0'
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
        //'x''x''x''x''\0'
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


        SDL_SetRenderDrawColor(pWindow->pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pWindow->pRenderer);
        PrintMenu(grid, pWindow, numberTile, numberBombs, &confirm);
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

BOOL GameOverScreen(Grid* grid, Window* pWindow, BOOL win) {

   
    SDL_SetRenderDrawColor(pWindow->pRenderer, 0, 0, 0, 255);

    SDL_RenderClear(pWindow->pRenderer);

    SDL_Event eventEnd;

    if (win == TRUE) { // Victoire  
        Colors(pWindow, 0, 0, 0, NULL);

        PrintTitle(grid, pWindow, pWindow->textureWin);



    }
    else { //Defaite
        Colors(pWindow, 0, 0, 0, NULL);

        PrintTitle(grid, pWindow, pWindow->textureLoose);
    
    }

    SDL_Rect retry = { 225 , (SCREEN_HEIGHT / 4) * 3, 150,100 };

    SDL_Rect quit = { 525 , (SCREEN_HEIGHT / 4) * 3, 150,100 };

    PrintSettings(grid, pWindow, pWindow->textureSettings[9], -150, (SCREEN_HEIGHT / 4) * 3 + 20);

    PrintSettings(grid, pWindow, pWindow->textureSettings[10], 150, (SCREEN_HEIGHT / 4) * 3 + 20);


    SDL_RenderPresent(pWindow->pRenderer);

    while (TRUE) {
        SDL_WaitEvent(&eventEnd);
        if (eventEnd.type == SDL_MOUSEBUTTONDOWN && eventEnd.button.button == SDL_BUTTON_LEFT) {

            if (BoxCollider(&retry, eventEnd.motion.x, eventEnd.motion.y)) {
                return TRUE;
            }
            if (BoxCollider(&quit, eventEnd.motion.x, eventEnd.motion.y)) {
                SDLQuit(pWindow);
                return FALSE;
            }
        }
    }


}


void CreateSurfaceAndTextureMenu(Window* pWindow) {


    SDL_Color white = { 255,255,255 };

    pWindow->fontSettings = TTF_OpenFont("pixel.ttf", 50);
    pWindow->fontTitle = TTF_OpenFont("pixel.ttf", 70);

    pWindow->surfaceMenuTitle = TTF_RenderText_Solid(pWindow->fontTitle, "MineSweeper", white);
    pWindow->textureMenuTitle = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceMenuTitle);

    pWindow->surfaceWin = TTF_RenderText_Solid(pWindow->fontTitle, "YOU WIN !!!", white);
    pWindow->textureWin = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceWin);

    pWindow->surfaceLoose = TTF_RenderText_Solid(pWindow->fontTitle, "YOU LOOSE !!!", white);
    pWindow->textureLoose = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceLoose);


    pWindow->surfaceSettings[0] = TTF_RenderText_Solid(pWindow->fontSettings, "<", white);
    pWindow->surfaceSettings[1] = TTF_RenderText_Solid(pWindow->fontSettings, "<<", white);
    pWindow->surfaceSettings[2] = TTF_RenderText_Solid(pWindow->fontSettings, "<<<", white);
    pWindow->surfaceSettings[3] = TTF_RenderText_Solid(pWindow->fontSettings, ">", white);
    pWindow->surfaceSettings[4] = TTF_RenderText_Solid(pWindow->fontSettings, ">>", white);
    pWindow->surfaceSettings[5] = TTF_RenderText_Solid(pWindow->fontSettings, ">>>", white);
    pWindow->surfaceSettings[6] = TTF_RenderText_Solid(pWindow->fontSettings, "Tiles Number", white);
    pWindow->surfaceSettings[7] = TTF_RenderText_Solid(pWindow->fontSettings, "Bombs Number", white);
    pWindow->surfaceSettings[8] = TTF_RenderText_Solid(pWindow->fontSettings, "Confirm", white);
    pWindow->surfaceSettings[9] = TTF_RenderText_Solid(pWindow->fontSettings, "Retry", white);
    pWindow->surfaceSettings[10] = TTF_RenderText_Solid(pWindow->fontSettings, "Quit", white);

    for (int i = 0; i < 11; i++) {
        pWindow->textureSettings[i] = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceSettings[i]);
    }
}


void CreateSurfaceAndTextureGame(Window* pWindow) {

    SDL_Color black = { 0,0,0 };

    pWindow->fontFlag = TTF_OpenFont("pixel.ttf", 20);

    
    pWindow->surfaceTitle = TTF_RenderText_Solid(pWindow->fontTitle, "MineSweeper", black);
    pWindow->textureTitle = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceTitle);

    pWindow->surfaceFlagTitle = TTF_RenderText_Solid(pWindow->fontFlag, "Remaining Flags", black);
    pWindow->textureFlagTitle = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceFlagTitle);


    SDL_Color text_color[8] = { {1,0,254}, {1,127,1},{254,0,0},{1,0,128}, {129,0,2}, {0,128,129},{0,0,0},{128,128,128} };

    for (int i = 0; i < 8; i++) {
        char number[2];
        number[0] = i + 1 + '0';
        number[1] = '\0';

        pWindow->surfaceNumbers[i] = TTF_RenderText_Solid(pWindow->fontNumbers, number, text_color[i]);
        pWindow->textureNumbers[i] = SDL_CreateTextureFromSurface(pWindow->pRenderer, pWindow->surfaceNumbers[i]);
    }


}

void SurfaceAndTextureFreeGame(Window* pWindow){

    SDL_FreeSurface(pWindow->surfaceTitle);
    SDL_DestroyTexture(pWindow->textureTitle);

    SDL_FreeSurface(pWindow->surfaceFlagTitle);
    SDL_DestroyTexture(pWindow->textureFlagTitle);


    for (int i = 0; i < 8; i++) {
        SDL_FreeSurface(pWindow->surfaceNumbers[i]);
        SDL_DestroyTexture(pWindow->textureNumbers[i]);
    }

}


void SurfaceAndTextureFreeMenu(Window* pWindow) {

    SDL_FreeSurface(pWindow->surfaceMenuTitle);
    SDL_DestroyTexture(pWindow->textureMenuTitle);

    for (int i = 0; i < 11; i++) {
        SDL_FreeSurface(pWindow->surfaceSettings[i]);
        SDL_DestroyTexture(pWindow->textureSettings[i]);
    }
}

void InitSDL(Window* pSDLWindow)
{
    //Etape 1: Initialisation de la SDL
    int iError = SDL_Init(SDL_INIT_VIDEO);
    if (iError != 0)
    {
        exit(1);
    }

    TTF_Init(); 

    //Etape 2: Construction de la Fenêtre (Window)
    pSDLWindow->pWindow = SDL_CreateWindow("MineSweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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


int main() {
 
    //INIT
    Window oSDLWindow;

    InitSDL(&oSDLWindow);

    CreateSurfaceAndTextureMenu(&oSDLWindow);


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



        /* RESOURCES 

        CreateSurface(&myGrid);
        CreateTexture(&myGrid);

        */


        int safezone = 1;

        myGrid.safezone = (safezone + 2) * (safezone + 2);
 
        MenuMineSweeper(&myGrid, &oSDLWindow);

        

        oSDLWindow.fontNumbers = TTF_OpenFont("pixel.ttf", (int)(55 / (myGrid.size / 10)) );

        CreateSurfaceAndTextureGame(&oSDLWindow);

        
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
                        PrintGridSDL(&myGrid, &oSDLWindow);
                        firststart = FALSE;
                    }

                    else if (event.button.button == SDL_BUTTON_RIGHT) {
                        PlaceFlag(&myGrid, coord);
                        PrintGridSDL(&myGrid, &oSDLWindow);

                    }

                    if (loose) {
                        winOrLoose = FALSE; 
                        round = FALSE;
                        EndGame(&myGrid);
                        PrintGridSDL(&myGrid, &oSDLWindow);
                        SDL_Delay(1500);


                    }

                    if (CheckWin(&myGrid)) {
                        winOrLoose = TRUE;
                        round = FALSE;
                        PrintGridSDL(&myGrid, &oSDLWindow);
                        SDL_Delay(1500);
                    }
                }
            }
        }



        if (!GameOverScreen(&myGrid, &oSDLWindow, winOrLoose)) {
            play = FALSE;
        }

        for (int i = 0; i < myGrid.size; i++) {
            free(myGrid.tiles[i]);
        }
        free(myGrid.tiles);

        SurfaceAndTextureFreeGame(&oSDLWindow); 

    }

    SurfaceAndTextureFreeMenu(&oSDLWindow);
    
    return 0;
}


