#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include "logika.h"
#include "grafika.h"

int main() {
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

    int score_list[3];
    read_score(score_list);

    int score = 0;
    SDL_Color textColor = {255,255,255,0};
    SDL_Color selectedColor = {255,0,0,0};

    SDL_Event event;
    bool quit = false;
    bool keyboard = false;
    int lives = 3;
    bool fly_flag = true;
    bool loaded_field = true;
    bool render_ball = false;
    bool game_start = false;
    bool end_game = false;
    bool victory_flag = false;
    int menu = 0;
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
    char line[100];

    int row = 0;
    if (field_file == NULL) {
        loaded_field = false;
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

    int menu_option = 1;
    
    while (!quit)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `event`
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT: // Pokud došlo k uzavření okna, nastav proměnnou `quit` na `true`
                quit = true;
                break;
            
            case SDL_MOUSEMOTION: //Pohyb myši
                if(!keyboard) {
                    pos_x =  event.motion.x;
                    if (pos_x-50 <= 25) pos_x = 75;
                    else if (pos_x+50 >= 775) pos_x = 725;
                }
                break;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (menu == 3 && menu_option ==2) {
                        lives--;
                        if (lives <= 0) lives = 99;
                    }
                    else if (keyboard) {
                        pos_x -=  25;
                        if (pos_x-45 <= 20) pos_x = 70;
                    }
                    break;
                
                case SDLK_RIGHT:
                    if (menu == 3 && menu_option ==2) {
                        lives++;
                        if (lives > 99) lives = 1;
                    }
                    else if (keyboard) {
                        pos_x +=  25;
                        if (pos_x+45 >= 780) pos_x = 730;
                    }
                    break;
                
                case SDLK_UP:
                    if (menu == 2 || menu == 3) {
                        menu_option--;
                        if (menu_option <= 0) menu_option = 3;
                    }
                    break;
                
                case SDLK_DOWN:
                    if (menu == 2 || menu == 3) {
                        menu_option++;
                        if (menu_option > 3) menu_option = 1;
                    }
                    break;

                case SDLK_RETURN:
                    if ((menu == 0 || menu == 1) && !game_start) {
                        menu++;
                    }
                    else if (menu == 2) {
                        switch (menu_option)
                        {
                        case 1:
                            game_start = true;
                            break;

                        case 2:
                            menu = 3;
                            break;

                        case 3:
                            quit = true;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    else if (menu == 3 ) {
                        switch (menu_option)
                        {
                        case 1:
                            keyboard = !keyboard;
                            break;


                        case 3:
                            menu = 2;
                            menu_option = 1;
                            break;
                        
                        default:
                            break;
                        }
                    }
                    break;

                case SDLK_SPACE:
                    if (lives > 0 && game_start) {
                        render_ball = true;
                        ball_x = pos_x + 50;
                        ball_y = 540;
                        ball.dir_x = 1;
                        ball.dir_y = -1;
                        fly_flag = true;
                    }
                    break;
                
                default:
                    break;
                }
                break;
            
            default:
                break;
            }
        }

        // Nastavení barvy vykreslování na černou
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (game_start == false) { //Hlavní menu hry
            draw_menu(renderer,font,menu,menu_option,textColor,selectedColor,lives,keyboard,score_list);
        }

        else { //Začátek hry
            draw_gui(renderer,font,score,lives);

            //Vykreslení okraje herního pole
            draw_bounds(renderer,res_width,res_height);

            //Vykreslování bloků
            if (loaded_field) {
                for (int i = 0; i < 15; i++) {
                    draw_blocks(renderer,field2[i],15);
                }
            }
            else {
                for (int i = 0; i < 15; i++) {
                    draw_blocks(renderer,field[i],15);
                }
            }

            // Vykreslení hráčské pálky
            SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
            SDL_Rect paddle = {pos_x-50,550,100,20}; //x_start, y_start, width, height
            SDL_RenderFillRect(renderer, &paddle);

            //BALL RENDERING
            if (ball_y >= 590) { //Podmínka pro prohru
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
                }

                //Kontrola kolize s bočními hranicemi herního pole
                if (ball_x + ball.texture.w >= res_width - 20 || ball_x <= 20) {
                    ball.dir_x *= -1;
                    hitbox_ball.dir_x *= -1;
                }

                if (ball_y + ball.texture.h >= res_height || ball_y <= 60) {
                    ball.dir_y *= -1;
                    hitbox_ball.dir_y *= -1;
                }

                //Odrážení míčku od boku pálky 
                if (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y > 550)) { 
                    if (SDL_HasIntersection(&hitbox_ball.texture, &paddle)) ball_y -= ((ball_y + ball.texture.h) - 550 + 2); 

                    ball.dir_x *= -1;
                    hitbox_ball.dir_x *= -1;
                    ball.dir_y *= -1;
                    hitbox_ball.dir_y *= -1;
                }

                //Kontrola kolize s bloky
                if (loaded_field) {
                    for (int i = 0; i < 15;i++) {
                        check_row_collision(&ball,hitbox_ball,field2[i],15,&score);
                    } 
                }
                else {
                    for (int i = 0; i < 15;i++) {
                        check_row_collision(&ball,hitbox_ball,field[i],15,&score);
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

            victory_flag = true;
            for (int x = 0; x < 15;x++) {
                for (int y = 0; y < 15;y++) {
                    if (!field2[x][y].broken) {
                        victory_flag = false;
                        break;
                    }
                }
            }
            
            //Vykreslování míčku
            // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            // SDL_RenderFillRect(renderer,&hitbox_ball.texture);
            if (render_ball) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer,&ball.texture);
            }
            else {
                draw_prompt(renderer,font,"Press SPACE to release the ball",200,450,450,50,textColor); 
            }
            if (victory_flag && !end_game) {
                draw_prompt(renderer,font,"You Won!",300,300,230,50,textColor);
            }
            else if (!victory_flag && end_game) {
                draw_prompt(renderer,font,"GAME OVER",300,300,230,50,textColor);
            }
        }

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }

    save_score(score_list,score);

    for (int i = 0; i < 15;i++) {
        free(field2[i]);
        free(field[i]);
    }

    TTF_CloseFont(font);

    quit_game(&window,&renderer);

    return 0;
}