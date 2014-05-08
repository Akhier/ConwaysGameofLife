#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

void logError(const std::string &message) {
    std::cout << message << " Error: " << SDL_GetError() << std::endl;
}

int main( int argc, char* args[] ){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logError("SDL_Init");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* tempcells = SDL_ConvertSurface(IMG_Load("Cells.bmp"), SDL_GetWindowSurface(window)->format, NULL);
    SDL_SetColorKey(tempcells, SDL_TRUE, SDL_MapRGB(tempcells->format, 255, 0, 255));
    SDL_Texture* cells = SDL_CreateTextureFromSurface(renderer, tempcells);
    SDL_FreeSurface(tempcells);
    tempcells = NULL;
    SDL_Rect alivePos, deadPos;
    alivePos.x = 0; deadPos.x = 8;
    alivePos.y = 0; deadPos.y = 0;
    alivePos.w = 8; deadPos.w = 8;
    alivePos.h = 8; deadPos.h = 8;
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                    quit = true;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, cells, &alivePos, NULL);
        SDL_RenderPresent(renderer);
    }

	return 0;
}
