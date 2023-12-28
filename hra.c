#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "logika.h"
#include "grafika.h"

// void swap(int* a, int* b) {
//     int tmp = *a;
//     *a = *b;
//     *b = tmp;
// }

// void sortThreeInts(int* a, int* b, int* c) {
//     if (*a > *b) {
//         swap(a, b);
//     }

//     if (*b > *c) {
//         swap(b, c);
//     }

//     if (*a > *b) {
//         swap(a, b);
//     }
// }

int main()
{
    int res_width = 800;
    int res_height = 600;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    game_init(&window,&renderer,res_width,res_height);

    //Otevírání fontu
    TTF_Font* font = TTF_OpenFont("../assets/VT323-Regular.ttf",32);
    if(font == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    // FILE * score_file = NULL;
    // score_file = fopen("../score.txt", "rt");
    // char line[100];

    // char** file_content = read_score("../score.txt");

    // for (int i = 0;i < 3;i++) {
    //     printf("Score[%d]:%s",i+1,file_content[i]);
    // }

    // fclose(score_file);

    int score = 0;
    char score_str[9] = "00000000";
    char lives_str[9] = "Lives: 0";

    SDL_Color textColor = {255,255,255,0};
    SDL_Surface* surfaceScore = TTF_RenderText_Solid(font, score_str, textColor);
    SDL_Surface* surfaceLives = TTF_RenderText_Solid(font, lives_str, textColor);

    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore);  
    SDL_Texture* livesText = SDL_CreateTextureFromSurface(renderer,surfaceLives);
    SDL_FreeSurface(surfaceScore);
    SDL_FreeSurface(surfaceLives);

    SDL_Event event;
    bool quit = false;
    bool keyboard = false;
    int lives = 3;
    bool end_game = false;
    bool render_ball = false;
    bool fly_flag = true;
    int pos_x = 400;
    int ball_x = 400;
    int ball_y = 520;
    Ball ball = {.texture.x = ball_x, .texture.y = ball_y, .texture.w = 10, .texture.h = 10, .dir_x = 0, .dir_y = 0};
    Ball hitbox_ball = {.texture.x = ball_x -1, .texture.y = ball_y -1, .texture.w = 12, .texture.h = 12, .dir_x = 0, .dir_y = 0};

    FILE * field_file = NULL;
    field_file = fopen("../starting-field.txt", "rt");
    Block** field = (Block**)malloc(15 * sizeof(Block*));
    Block** field2 = (Block**)malloc(15 * sizeof(Block*));

    for (int i = 0; i< 15; i++) {
        field2[i] = generate_blocks(15,5,i);
    }
    char * line[100];

    int row = 0;
    if (field_file == NULL) {
        printf("Herní pole nebylo načteno\n");
    }
    while (fgets(line, sizeof(line), field_file) != NULL) {
        char* token = strtok(line, "|");
        int column = 0;

        while (token != NULL && column < 15) {
            field2[row][column].health = atoi(token);

            if (field2[row][column].health == 0 && token[0] != '0') {
                printf("Chyba čtení: %s\n", token);
            }

            token = strtok(NULL, "|");
            column++;
        }
        row++;
    }


    fclose(field_file);


    for (int i = 0; i< 15; i++) {
        field[i] = generate_blocks(15,5,i);
    }
    for (int i = 0; i< 15; i++) {
        field[i][7].broken = true;
    }
    // Block * green_blocks = generate_blocks(15,5,0);
    // Block * blocks = generate_blocks(15,2,1);

    while (!quit)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `event`
        while (SDL_PollEvent(&event))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `quit` na `true`
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if(event.type == SDL_MOUSEMOTION && !keyboard) {
                pos_x =  event.motion.x;
                if (pos_x-50 <= 25) pos_x = 75;
                else if (pos_x+50 >= 775) pos_x = 725;
            }
            if (event.type == SDL_KEYDOWN && keyboard) {
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
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE  && lives > 0) {
                render_ball = true;
                ball_x = pos_x + 50;
                ball_y = 540;
                ball.dir_x = 1;
                ball.dir_y = -1;
                fly_flag = true;
            }
        }

        // Nastavení barvy vykreslování na černou
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Vykreslení skóre
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_Rect scoreBox = {20,0,100,30};
        SDL_RenderFillRect(renderer,&scoreBox);
        if (score > 99999999) score = 0;
        char *formattedScore = format_score(score);
        surfaceScore = TTF_RenderText_Solid(font, formattedScore, textColor);
        SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore); 
        SDL_RenderCopy(renderer,scoreText,NULL,&scoreBox);
        SDL_FreeSurface(surfaceScore);
        free(formattedScore);

        //Vykreslení životů
        SDL_Rect livesBox = {300,0,100,30};
        SDL_RenderFillRect(renderer,&livesBox);
        char *formattedLives = format_lives(lives);
        surfaceLives = TTF_RenderText_Solid(font, formattedLives, textColor);
        SDL_Texture* livesText = SDL_CreateTextureFromSurface(renderer,surfaceLives); 
        SDL_RenderCopy(renderer,livesText,NULL,&livesBox);
        SDL_FreeSurface(surfaceLives);
        free(formattedLives);

        //Vykreslení okraje herního pole
        draw_bounds(renderer,res_width,res_height);

        //Vykreslování bloků
        for (int i = 0; i < 15; i++) {
            draw_blocks(renderer,field2[i],15);
        }
        // draw_blocks(renderer,green_blocks,15);
        // draw_blocks(renderer,blocks,15);

        // Vykreslení hráčské pálky
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
        SDL_Rect paddle = {pos_x-50,550,100,20}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &paddle);

        //BALL RENDERING
        //Podmínka pro prohru
        if (ball_y >= 590) {
            fly_flag = false;
            lives--;
            ball_x = 10;
            ball_y = 300;
            ball.dir_x = 0;
            ball.dir_y = 0;
            if (lives == 0) {
                end_game = true;
            }
        }

        if (fly_flag) {

            //Odrážení míčku od vrchu pálky
            if (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y < 550)) {
                // printf("dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
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
                // printf("AFTER dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
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
                // printf("texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                // printf("dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
                if (SDL_HasIntersection(&hitbox_ball.texture, &paddle)) ball_y -= ((ball_y + ball.texture.h) - 550 + 2); 

                //printf("AFTER:6 texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                ball.dir_x *= -1;
                hitbox_ball.dir_x *= -1;
                ball.dir_y *= -1;
                hitbox_ball.dir_y *= -1;
                //printf("AFTER dirx: %d,diry:%d\n",ball.dir_x, ball.dir_y);
            }

            //Kontrola kolize s bloky
            for (int i = 0; i < 15;i++) {
                check_row_collision(&ball,hitbox_ball,field2[i],15,&score);
            } 

            // check_row_collision(&ball,hitbox_ball,green_blocks,15,&score);
            // check_row_collision(&ball,hitbox_ball,blocks,15,&score);

            //Pohyb míčku
            ball.texture.x = ball_x;
            hitbox_ball.texture.x = ball_x -1;
            ball_x += ball.dir_x * 3;

            ball.texture.y = ball_y;
            hitbox_ball.texture.y = ball_y -1;
            ball_y += ball.dir_y * 3;
            
        }
        
        //Vykreslování míčku
        // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        // SDL_RenderFillRect(renderer,&hitbox_ball.texture);
        if (render_ball) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer,&ball.texture);
        }

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(scoreText);
    SDL_DestroyTexture(livesText);

    for (int i = 0; i < 15;i++) {
        free(field2[i]);
    } 
    // free(green_blocks);
    // free(blocks);

    TTF_CloseFont(font);
    //free(file_content);

    quit_game(&window,&renderer);

    return 0;
}