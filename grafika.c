#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "grafika.h"

Block * generate_blocks (int amount, int health, int row ) {
    Block * blocks = (Block *) malloc (sizeof(Block) * amount);

    for (int i = 0;i < amount;i++) { 
        blocks[i].texture.x = i * 50 + 25;
        blocks[i].texture.y = 200;
        blocks[i].texture.w = 50;
        blocks[i].texture.h = 20;
        blocks[i].broken = false;
        blocks[i].health = health;
    }
    return blocks;
}


void draw_blocks (SDL_Renderer * renderer, Block * blocks) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < 15; i++) {
        if (blocks[i].broken == true) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        } 
        else if (blocks[i].health == 11){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else if (blocks[i].health == 8){
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        }
        else if (blocks[i].health == 5){
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        }
        
        SDL_RenderFillRect(renderer, &blocks[i].texture);
    }
}

void draw_bounds (SDL_Renderer * renderer,int res_width, int res_height) {
    SDL_SetRenderDrawColor(renderer, 68, 74, 67, 255);
    SDL_Rect sideBoundL = {0,40,25,res_height}; 
    SDL_RenderFillRect(renderer, &sideBoundL);
    SDL_Rect sideBoundR = {775,40,25,res_height};
    SDL_RenderFillRect(renderer, &sideBoundR);
    SDL_Rect topBound = {20,40,res_width - 40,20};
    SDL_RenderFillRect(renderer,&topBound);
    SDL_SetRenderDrawColor(renderer, 77, 188, 215, 255);
    SDL_Rect sideBound_B = {0,550,25,20};
    SDL_RenderFillRect(renderer,&sideBound_B);
    SDL_SetRenderDrawColor(renderer, 218, 52, 16, 255);
    SDL_Rect sideBound_R = {775,550,25,20};
    SDL_RenderFillRect(renderer,&sideBound_R);
}