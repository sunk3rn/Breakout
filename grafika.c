#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "grafika.h"
#include "logika.h"

Block * generate_blocks (int amount, int health, int row ) {
    Block * blocks = (Block *) malloc (sizeof(Block) * amount);

    for (int i = 0;i < amount;i++) { 
        blocks[i].texture.x = i * 50 + 25;
        blocks[i].texture.y = 120 + row * 20;
        blocks[i].texture.w = 50;
        blocks[i].texture.h = 20;
        blocks[i].broken = false;
        blocks[i].health = health;
    }
    return blocks;
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

void draw_blocks (SDL_Renderer * renderer, Block * blocks, int amount) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < amount; i++) {
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
        else if (blocks[i].health <= -1){
            SDL_SetRenderDrawColor(renderer, 255, 192, 203, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        }
        
        SDL_RenderFillRect(renderer, &blocks[i].texture);
    }
}


void check_row_collision(Ball * ball, Ball hitbox_ball, Block * blocks,int amount, int * score) {
    for (int i = 0; i < amount; i++) {
        if (SDL_HasIntersection(&hitbox_ball.texture, &blocks[i].texture) && !blocks[i].broken) {
            if (hitbox_ball.texture.y + hitbox_ball.texture.h / 2 < blocks[i].texture.y + blocks[i].texture.h / 2) {
                ball->dir_y = -1;
            } else {
                ball->dir_y = 1;
            }

            printf("Score++\n");
            (*score) += 1;

            blocks[i].health -=1;

            if (blocks[i].health <= 0) {
                blocks[i].broken = true;
            }
        }
    }
}

void draw_gui(SDL_Renderer * renderer,TTF_Font* font,int score,int lives) {
    char score_str[9] = "00000000";
    char lives_str[9] = "Lives: 0";
    SDL_Color textColor = {255,255,255,0};
    SDL_Surface* surfaceScore = TTF_RenderText_Solid(font, score_str, textColor);
    SDL_Surface* surfaceLives = TTF_RenderText_Solid(font, lives_str, textColor);

    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore);  
    SDL_Texture* livesText = SDL_CreateTextureFromSurface(renderer,surfaceLives);
    SDL_FreeSurface(surfaceScore);
    SDL_FreeSurface(surfaceLives);
    //Vykreslení skóre
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_Rect scoreBox = {20,0,100,30};
    SDL_RenderFillRect(renderer,&scoreBox);
    if (score > 99999999) score = 0;
    char *formattedScore = format_score(score);
    surfaceScore = TTF_RenderText_Solid(font, formattedScore, textColor);
    scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore); 
    SDL_RenderCopy(renderer,scoreText,NULL,&scoreBox);
    SDL_FreeSurface(surfaceScore);
    free(formattedScore);

    //Vykreslení životů
    SDL_Rect livesBox = {300,0,100,30};
    SDL_RenderFillRect(renderer,&livesBox);
    char *formattedLives = format_lives(lives);
    surfaceLives = TTF_RenderText_Solid(font, formattedLives, textColor);
    livesText = SDL_CreateTextureFromSurface(renderer,surfaceLives); 
    SDL_RenderCopy(renderer,livesText,NULL,&livesBox);
    SDL_FreeSurface(surfaceLives);
    free(formattedLives);
}