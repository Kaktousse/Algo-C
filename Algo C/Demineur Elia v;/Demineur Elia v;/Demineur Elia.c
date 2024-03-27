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





void Colors(SDL_Surface** screenSurface, int blue, int green, int red, const SDL_Rect* rect) {

    SDL_FillRect(*screenSurface, rect, SDL_MapRGB((*screenSurface)->format, blue, green, red));
  
}

void PrintFlags(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {


    TTF_Font* font = TTF_OpenFont("pixel.ttf", 20);

    SDL_Surface* textFlagTitle;
    SDL_Texture* textureFlagTitle;
    SDL_Color titleColor = { 0,0,0 };

    textFlagTitle = TTF_RenderText_Solid(font, "Remaining Flags", titleColor);

    textureFlagTitle = SDL_CreateTextureFromSurface(*renderer, textFlagTitle);


    int height, width;

    SDL_QueryTexture(textureFlagTitle, NULL, NULL, &width, &height);

    SDL_Rect flagTitleRect = { 10 , 10, width, height };

    SDL_BlitSurface(textFlagTitle, NULL, *screenSurface, &flagTitleRect);

    SDL_FreeSurface(textFlagTitle);
    SDL_DestroyTexture(textureFlagTitle);


    TTF_Font* flagfont = TTF_OpenFont("pixel.ttf", 20);

    SDL_Surface* textFlag;
    SDL_Texture* textureFlag;

    char flags[4];
    flags[0] = (char)(int)(grid->flag / 100) + '0';
    flags[1] = (char)((int)(grid->flag / 10) - ((int)(grid->flag / 100) * 10)) + '0';
    flags[2] = (char)(grid->flag - (((int)(grid->flag / 10) - ((int)(grid->flag / 100) * 10)) * 10) - ((int)(grid->flag / 100) * 100)) + '0';
    flags[3] = '\0';


    textFlag = TTF_RenderText_Solid(flagfont, flags , titleColor);

    textureFlag = SDL_CreateTextureFromSurface(*renderer, textFlag);

    SDL_QueryTexture(textureFlag, NULL, NULL, &width, &height);

    SDL_Rect flagRect = { 10, 50, width, height };

    SDL_BlitSurface(textFlag, NULL, *screenSurface, &flagRect);

    SDL_FreeSurface(textFlag);
    SDL_DestroyTexture(textureFlag);


}

void InitGridSDL(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {


    Colors(screenSurface, 218, 218, 218, NULL);

    TTF_Font* font = TTF_OpenFont("pixel.ttf", 70);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 0,0,0 };

    textTitle = TTF_RenderText_Solid(font, "MineSweeper", titleColor);

    texture = SDL_CreateTextureFromSurface(*renderer, textTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_BlitSurface(textTitle, NULL, *screenSurface, &newrect);

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


void PrintSquare(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** surface, Tile* tile, Grid* grid) {

    TTF_Font* font = TTF_OpenFont("arial.ttf", (int) 55 / (grid->size/10));


    SDL_Surface* text;
    SDL_Texture* texture;

    char number[2];
    number[0] = (char)tile->bombAround + '0';
    number[1] = '\0';

    text = TTF_RenderText_Solid(font, number, tile->color);

    texture = SDL_CreateTextureFromSurface(*renderer, text);


    int height = tile->rect.h , width = tile->rect.w;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { tile->rect.x + (tile->rect.w/4), tile->rect.y+(tile->rect.h / 8), width, height};

    SDL_BlitSurface(text, NULL, *surface, &newrect);

    SDL_FreeSurface(text);

}



void PrintGridSDL(Grid* grid, SDL_Surface** screenSurface, SDL_Window** window, SDL_Renderer** renderer) {


    Colors(screenSurface, 218, 218, 218, NULL);

    TTF_Font* font = TTF_OpenFont("pixel.ttf", 70);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 0,0,0 };

    textTitle = TTF_RenderText_Solid(font, "MineSweeper", titleColor);

    texture = SDL_CreateTextureFromSurface(*renderer, textTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 10, width, height };

    SDL_BlitSurface(textTitle, NULL, *screenSurface, &newrect);

    SDL_FreeSurface(textTitle);
    SDL_DestroyTexture(texture);





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

                PrintSquare(window, renderer, screenSurface, &grid->tiles[i][j], grid);

            }

        }
    }
    
    PrintFlags(grid, screenSurface, window, renderer);

    SDL_UpdateWindowSurface(*window);


}

void PrintTitle(Grid* grid, SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screenSurface) {

    TTF_Font* font = TTF_OpenFont("pixel.ttf", 80);


    SDL_Surface* textTitle;
    SDL_Texture* texture;
    SDL_Color titleColor = { 255,255,255 };

    textTitle = TTF_RenderText_Solid(font, "MineSweeper", titleColor);

    texture = SDL_CreateTextureFromSurface(*renderer, textTitle);


    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2, 30, width, height };

    SDL_BlitSurface(textTitle, NULL, *screenSurface, &newrect);

    SDL_FreeSurface(textTitle);
    SDL_DestroyTexture(texture);
}


void PrintSettings(Grid* grid, SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screenSurface, const char* title, int xpos, int ypos, int size) {

    TTF_Font* font = TTF_OpenFont("pixel.ttf", size);

    SDL_Surface* txtSurface;
    SDL_Texture* texture;

    SDL_Color white = { 255,255,255 };

    txtSurface = TTF_RenderText_Solid(font, title, white);

    texture = SDL_CreateTextureFromSurface(*renderer, txtSurface);



    int height, width;

    SDL_QueryTexture(texture, NULL, NULL, &width, &height);

    SDL_Rect newrect = { SCREEN_WIDTH / 2 - width / 2 + xpos, ypos, width, height };

    SDL_BlitSurface(txtSurface, NULL, *screenSurface, &newrect);


    SDL_FreeSurface(txtSurface);
    SDL_DestroyTexture(texture);

}

void PrintMenu(Grid* grid, SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screenSurface, char* numberTile, char* numberBomb, SDL_Rect* confirm) {

    PrintTitle(grid, window, renderer, screenSurface);

    int ypos;
    int height, width;

    PrintSettings(grid, window, renderer, screenSurface, "Tiles Number", 0, SCREEN_HEIGHT / 4, 50);

    PrintSettings(grid, window, renderer, screenSurface, "<", -60, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, "<<", -150, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, numberTile, 0, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, ">", 60, SCREEN_HEIGHT / 4 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, ">>", 150, SCREEN_HEIGHT / 4 + 80, 50);


    PrintSettings(grid, window, renderer, screenSurface, "Bombs Number", 0, SCREEN_HEIGHT / 2, 50);


    PrintSettings(grid, window, renderer, screenSurface, "<", -60, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, "<<", -150, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, "<<<", -250, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, numberBomb, 0, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, ">", 60, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, ">>", 150, SCREEN_HEIGHT / 2 + 80, 50);

    PrintSettings(grid, window, renderer, screenSurface, ">>>", 250, SCREEN_HEIGHT / 2 + 80, 50);


    Colors(screenSurface, 50, 50, 50, confirm);

    PrintSettings(grid, window, renderer, screenSurface, "Confirm", 0, (SCREEN_HEIGHT / 4) * 3 + 20, 50);



}





void MenuMineSweeper(Grid* grid, SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** screenSurface) {



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


        int maxBomb = 10 + (tileCount - 10) * 5;

        if (tileCount <= 99) {
            if (tileSize == 4) {
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
            numberTile = (char*)realloc(numberTile, sizeof(char) * 4);
            if (numberTile == NULL) {
                exit(1);
            }
            numberTile[0] = '1';
            numberTile[1] = '0';
            numberTile[2] = '0';
            numberTile[3] = '\0';
        }




        if (bombCount <= 9) {
            if (bombSize != 2) {
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
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 3);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }
            numberBombs[0] = (char)(int)(bombCount / 10) + '0';
            numberBombs[1] = (char)(bombCount - ((int)(bombCount / 10) * 10)) + '0';
            numberBombs[2] = '\0';
        }
        else if (bombCount > 99){
            if (bombSize != 4) {
                numberBombs = (char*)realloc(numberBombs, sizeof(char) * 4);
                if (numberBombs == NULL) {
                    exit(1);
                }
            }
            numberBombs[0] = (char) (int)(bombCount / 100 ) +'0';
            numberBombs[1] = (char) ((int)(bombCount / 10)  - ( (int)(bombCount / 100) *10 ) ) + '0';
            numberBombs[2] = (char)(bombCount - (((int)(bombCount / 10) - ( (int)(bombCount / 100) * 10)) * 10 )- ((int)(bombCount / 100) * 100) )+ '0';
            numberBombs[3] = '\0';
        }



        Colors(screenSurface, 0, 0, 0, NULL);

        SDL_Rect leftRectTiles = { 380, SCREEN_HEIGHT / 4 + 80, 30,50 };
        SDL_Rect rightRectTiles = { 490, SCREEN_HEIGHT / 4 + 80, 30,50 };


        SDL_Rect doubleLeftRectTiles = { 275, SCREEN_HEIGHT / 4 + 80, 50,50 };
        SDL_Rect doubleRightRectTiles = { 575, SCREEN_HEIGHT / 4 + 80, 50,50 };


        SDL_Rect leftRectBombs = { 380, SCREEN_HEIGHT / 2 + 80, 30,50 };
        SDL_Rect rightRectBombs = { 490, SCREEN_HEIGHT / 2 + 80, 30,50 };

        SDL_Rect doubleLeftRectBombs = { 275, SCREEN_HEIGHT / 2 + 80, 50,50 };
        SDL_Rect doubleRightRectBombs = { 575, SCREEN_HEIGHT / 2+ 80, 50,50 };

        SDL_Rect tripleLeftRectBombs = { 160, SCREEN_HEIGHT / 2 + 80, 80,50 };
        SDL_Rect tripleRightRectBombs = { 660, SCREEN_HEIGHT / 2 + 80, 80,50 };


        SDL_Rect confirm = { 300 , (SCREEN_HEIGHT / 4) * 3, 300,100 };

        PrintMenu(grid, window, renderer, screenSurface, numberTile, numberBombs, &confirm);


        SDL_UpdateWindowSurface(*window);

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
                    if (tileCount < 90) {
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
                    if (bombCount >= 20) {
                        bombCount -= 10;
                    }
                }

                if (BoxCollider(&doubleRightRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount < maxBomb-10) {
                        bombCount += 10;
                    }
                }
                if (BoxCollider(&tripleLeftRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount >= 200) {
                        bombCount -= 100;
                    }
                }

                if (BoxCollider(&tripleRightRectBombs, eventMenu.motion.x, eventMenu.motion.y)) {
                    if (bombCount < maxBomb-100) {
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


void QuitGame(SDL_Window** window) {

    SDL_DestroyWindow(*window);

    TTF_Quit();
    SDL_Quit();


}


int main() {
 

    BOOL play = TRUE;

    while (play) {
        system("cls");


        Grid myGrid;

        
        BOOL firststart = TRUE;
        Coord coord;

        SDL_Event event;
        BOOL round = TRUE;
        BOOL loose = FALSE;
        
        SDL_Renderer* myRenderer = NULL;
        SDL_Window* myWindow = SetWindow(&myRenderer);

        SDL_Surface* mySurface = SetSurface(&myWindow);


        system("cls");

 
        MenuMineSweeper(&myGrid, &myWindow, &myRenderer, &mySurface);

        myGrid.safezone = 1;

        int safeZoneSquare = (myGrid.safezone + 2) * (myGrid.safezone + 2);

        myGrid.remainingTiles = (myGrid.size * myGrid.size) - myGrid.bombCount;

        printf("%d %d", myGrid.size, myGrid.bombCount);
        srand(time(NULL));
        InitGrid(&myGrid);
     
        myGrid.flag = myGrid.bombCount;

        InitGridSDL(&myGrid, &mySurface, &myWindow, &myRenderer);


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