#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

void logError(const std::string &message) {
    std::cout << message << " Error: " << SDL_GetError() << std::endl;
}

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
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logError("SDL_Init");
        return 1;
    }
    srand(time(NULL));
    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* tempcells = SDL_ConvertSurface(IMG_Load("Cells.bmp"), SDL_GetWindowSurface(window)->format, NULL);
    SDL_SetColorKey(tempcells, SDL_TRUE, SDL_MapRGB(tempcells->format, 255, 0, 255));
    SDL_Texture* cells = SDL_CreateTextureFromSurface(renderer, tempcells);
    SDL_FreeSurface(tempcells);
    tempcells = NULL;
    SDL_Rect alivePos, deadPos, destPos;
    alivePos.x = 0; deadPos.x = 8; destPos.x = 0;
    alivePos.y = 0; deadPos.y = 0; destPos.y = 0;
    alivePos.w = 8; deadPos.w = 8; destPos.w = 8;
    alivePos.h = 8; deadPos.h = 8; destPos.h = 8;
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
        SDL_RenderClear(renderer);
        if (continualSteps){
            stepLife(gridmap);
        }
        for (int column = 0; column < 100; ++column){
            for (int row = 0; row < 100; ++row){
                destPos.x = row * 8;
                destPos.y = column * 8;
                SDL_RenderCopy(renderer, cells, gridmap[row][column]?&alivePos:&deadPos, &destPos);
            }
        }
        SDL_RenderPresent(renderer);
    }

	return 0;
}
