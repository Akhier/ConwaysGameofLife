#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "drw_sdl2.h"

void setMapToDead(bool (&gridmap)[100][100]){
    for (int column = 0; column < 100; ++column){
        for (int row = 0; row < 100; ++row){
            gridmap[row][column] = false;
        }
    }
}

bool randBool(){
    return rand() % 2 == 1;
}

void setMapRandomly(bool (&gridmap)[100][100]){
    for (int column = 0; column < 100; ++column){
        for (int row = 0; row < 100; ++row){
            gridmap[row][column] = randBool() ? randBool(): false;
        }
    }
}

void invertMapTile(bool(&gridmap)[100][100]){
    int row, column;
    SDL_GetMouseState(&row, &column);
    row = row / 8;
    column = column / 8;
    gridmap[row][column] = !gridmap[row][column];
}

void stepLife(bool (&gridmap)[100][100]){
    int livecount[100][100] = {0};
    for (int column = 0; column < 100; ++column){
        for (int row = 0; row < 100; ++row){
            if (gridmap[row][column]){
                if (row != 0){
                    if (column != 0){
                        ++livecount[row - 1][column - 1];
                    }
                    if (column != 99){
                        ++livecount[row - 1][column + 1];
                    }
                    ++livecount[row - 1][column];
                }
                if (row != 99){
                    if (column != 0){
                        ++livecount[row + 1][column - 1];
                    }
                    if (column != 99){
                        ++livecount[row + 1][column + 1];
                    }
                    ++livecount[row + 1][column];
                }
                if (column != 0){
                    ++livecount[row][column - 1];
                }
                if (column != 99){
                    ++livecount[row][column + 1];
                }
            }
        }
    }
    for (int column = 0; column < 100; ++column){
        for (int row = 0; row < 100; ++row){
            if (livecount[row][column] < 2){
                gridmap[row][column] = false;
            }
            else if (livecount[row][column] == 3){
                gridmap[row][column] = true;
            }
            else if (livecount[row][column] > 3){
                gridmap[row][column] = false;
            }
        }
    }
}

int main( int argc, char* args[] ){
    DrW_SDL2 drw;
    srand(time(NULL));
    drw.createWindow("Conway's Game of Life", 800, 800, false);
    int Cells = drw.createTexture("Cells.png");
    int alive = drw.setupTileset(Cells, Rect(0, 0, 8, 8));
    int dead = drw.setupTileset(Cells, Rect(8, 0, 8, 8));
    Rect destPos = Rect(0, 0, 8, 8);
    bool gridmap[100][100];
    setMapToDead(gridmap);
    SDL_Event e;
    bool quit = false;
    bool continualSteps = false;
    while (!quit){
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_SPACE:
                        stepLife(gridmap);
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_p:
                        continualSteps = !continualSteps;
                        break;
                    default:
                        break;
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    invertMapTile(gridmap);
                }
                if(e.button.button == SDL_BUTTON_MIDDLE){
                    setMapRandomly(gridmap);
                }
                if(e.button.button == SDL_BUTTON_RIGHT){
                    setMapToDead(gridmap);
                }
            }
        }
        drw.renderclear();
        if (continualSteps){
            stepLife(gridmap);
        }
        for (int column = 0; column < 100; ++column){
            for (int row = 0; row < 100; ++row){
                destPos.X = row * 8;
                destPos.Y = column * 8;
                drw.renderTexture(Cells, gridmap[row][column] ? drw.getSourceRect(Cells, alive) : drw.getSourceRect(Cells, dead), destPos);
            }
        }
        drw.renderpresent();
    }
	return 0;
}
