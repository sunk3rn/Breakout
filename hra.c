#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "logika.h"


int main()
{
    int res_width = 800;
    int res_height = 600;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    game_init(&window,&renderer,res_width,res_height);

    TTF_Font* font = TTF_OpenFont("../assets/VT323-Regular.ttf",32);
    if(font == NULL) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }

    int score = 0;
    char score_str[9] = "00000000";

    SDL_Color textColor = {255,255,255,0};
    SDL_Surface* surfaceText = TTF_RenderText_Solid( font, "hello world", textColor );
    SDL_Surface* surfaceScore = TTF_RenderText_Solid( font, score_str, textColor );

    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer,surfaceText);  
    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore);  
    SDL_FreeSurface(surfaceScore);
    SDL_FreeSurface(surfaceText);

    SDL_Event event;
    bool quit = false;
    bool keyboard = false;
    
    int pos_x = 400;
    bool movement = false;
    int ball_x = 400;
    int ball_y = 520;
    Ball ball = {.texture.x = ball_x, .texture.y = ball_y, .texture.w = 10, .texture.h = 10, .dir_x = 1, .dir_y = -1};
    Ball hitbox_ball = {.texture.x = ball_x -1, .texture.y = ball_y -1, .texture.w = 12, .texture.h = 12, .dir_x = 1, .dir_y = 1};
    bool fly_flag = true;

    Block * green_blocks = (Block *) malloc (sizeof(Block) * 15);

    for (int i = 0;i < 15;i++) { 
        green_blocks[i].texture.x = i * 50 + 25;
        green_blocks[i].texture.y = 200;
        green_blocks[i].texture.w = 50;
        green_blocks[i].texture.h = 20;
        green_blocks[i].broken = false;
    }

    while (!quit)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `event`
        while (SDL_PollEvent(&event))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `quit` na `true`
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_MOUSEMOTION && !keyboard) {
                pos_x =  event.motion.x;
                if (pos_x-50 <= 25) pos_x = 75;
                else if (pos_x+50 >= 775) pos_x = 725;
            }
            if (SDL_KEYDOWN && keyboard) {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    pos_x -=  15;
                    if (pos_x-45 <= 20) pos_x = 70;
                    break;
                
                case SDLK_RIGHT:
                    pos_x +=  15;
                    if (pos_x+45 >= 780) pos_x = 730;
                    break;
                
                default:
                    break;
                }
            }
        }

        // Nastavení barvy vykreslování na černou
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Vykreslení pozadí
        SDL_RenderClear(renderer);

        //Vykreslení skóre
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_Rect scoreBox = {20,0,100,30};
        SDL_RenderFillRect(renderer,&scoreBox);
        if (score > 99999999) score = 0;
        surfaceScore = TTF_RenderText_Solid( font, format_score(score), textColor );
        SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore); 
        SDL_RenderCopy(renderer,scoreText,NULL,&scoreBox);

        //Vykreslení okraje herního pole
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

        //Vykreslování bloků
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i < 15; i++) {
            if (green_blocks[i].broken == true) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }
            
            SDL_RenderFillRect(renderer, &green_blocks[i].texture);
        }

        // Vykreslení hráčské pálky
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
        SDL_Rect paddle = {pos_x-50,550,100,20}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &paddle);
        SDL_RenderCopy(renderer,textureText,NULL,&paddle);

        //BALL RENDERING
        if (ball_y >= 590) {
            fly_flag = false;
        }

        if (fly_flag) {

            //Odrážení míčku od vrchu pálky
            if (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y < 550)) {
                printf("dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
                ball.dir_y = -1;
                hitbox_ball.dir_y = -1;
                if (ball_x > paddle.x + 50) {
                    ball.dir_x = 1;
                    hitbox_ball.dir_x = 1;
                }
                else {
                    ball.dir_x = -1;
                    hitbox_ball.dir_x = -1;
                }
                printf("AFTER dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
            }

            //Kontrola kolize s bočními hranicemi herního pole
            if ( ball_x + ball.texture.w >= res_width - 20 || ball_x <= 20) {
                ball.dir_x *= -1;
                hitbox_ball.dir_x *= -1;
            }

            if ( ball_y + ball.texture.h >= res_height || ball_y <= 60 ) {
                ball.dir_y *= -1;
                hitbox_ball.dir_y *= -1;
            }

            //Boční odrážení míčku od pálky 
            if (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y > 550)) { 
                printf("texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                printf("dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
                if (SDL_HasIntersection(&hitbox_ball.texture, &paddle)) {
                    ball_y -= ((ball_y + ball.texture.h) - 550 + 2) ; 
                }
                printf("AFTER:6 texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                ball.dir_x *= -1;
                hitbox_ball.dir_x *= -1;
                ball.dir_y *= -1;
                hitbox_ball.dir_y *= -1;
                printf("AFTER dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
            }

            //Kontrola kolize s bloky
            for (int i = 0;i < 15;i++) {
                if (SDL_HasIntersection(&hitbox_ball.texture, &green_blocks[i].texture) && !green_blocks[i].broken) {
                    score++;
                    green_blocks[i].broken = true;
                    ball.dir_y *= -1;
                    hitbox_ball.dir_y *= -1;

                }
                
            }

            //Pohyb míčku
            ball.texture.x = ball_x;
            hitbox_ball.texture.x = ball_x -1;
            ball_x += ball.dir_x * 3;

            ball.texture.y = ball_y;
            hitbox_ball.texture.y = ball_y -1;
            ball_y += ball.dir_y * 3;
            
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer,&hitbox_ball.texture);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer,&ball.texture);

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }

    free(green_blocks);

    quit_game(&window,&renderer);

    return 0;
}