#include <SDL2/SDL.h>  // Vložení hlavního hlavičkového souboru SDL
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect texture;
    int dir_x;
    int dir_y;
} Ball;

void game_init (SDL_Window ** window, SDL_Renderer ** renderer, int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);   // Inicializace SDL	
    TTF_Init();
    // Vytvoření okna
    *window = SDL_CreateWindow(
        "Breakout",  // Název
        100,                // Souřadnice x
        100,                // Souřadnice y
        width,                // Šířka
        height,                // Výška
        SDL_WINDOW_SHOWN    // Okno se má po vytvoření rovnou zobrazit
    );
    // Vytvoření kreslítka
    *renderer = SDL_CreateRenderer(
        *window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

}

void quit_game(SDL_Window ** window, SDL_Renderer ** renderer) {
    // Uvolnění prostředků
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_Quit();
    TTF_Quit();
}

int main()
{
    int res_width = 800;
    int res_height = 600;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    game_init(&window,&renderer,res_width,res_height);

    if(TTF_Init() == -1) {
        printf( "Unable to render text surface! SDL2_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }
    else {
        printf("SDL2_ttf working fine\n");
    }

    TTF_Font* font = TTF_OpenFont("../assets/VT323-Regular.ttf",32);
    if(font == NULL) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }

    SDL_Color textColor = {255,255,255,0};
    SDL_Surface* surfaceText = TTF_RenderText_Solid( font, "hello world", textColor );
    SDL_Surface* surfaceScore = TTF_RenderText_Solid( font, "0000000", textColor );

    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer,surfaceText);  
    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore);  
    SDL_FreeSurface(surfaceText);
    SDL_FreeSurface(surfaceScore);

    SDL_Event event;
    bool quit = false;
    bool keyboard = true;
    int pos_x = 400;
    bool movement = false;
    int ball_x = 400;
    int ball_y = 520;
    Ball ball = {.texture.x = ball_x, .texture.y = ball_y, .texture.w = 10, .texture.h = 10, .dir_x = 1, .dir_y = -1};
    Ball hitbox_ball = {.texture.x = ball_x -1, .texture.y = ball_y -1, .texture.w = 12, .texture.h = 12, .dir_x = 1, .dir_y = 1};
    bool fly_flag = true;

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
                if (pos_x-50 <= 50) pos_x = 80;
                else if (pos_x+50 >= 750) pos_x = 720;
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
        SDL_RenderCopy(renderer,scoreText,NULL,&scoreBox);

        //Vykreslení okraje herního pole
        SDL_SetRenderDrawColor(renderer, 68, 74, 67, 255);
        SDL_Rect sideBoundL = {0,0,20,res_height}; 
        SDL_RenderFillRect(renderer, &sideBoundL);
        SDL_Rect sideBoundR = {780,0,20,res_height};
        SDL_RenderFillRect(renderer, &sideBoundR);
        SDL_Rect topBound = {20,40,res_width - 40,20};
        SDL_RenderFillRect(renderer,&topBound);

        // Vykreslení hráčské pálky
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
        SDL_Rect paddle = {pos_x-50,550,100,20}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &paddle);
        SDL_RenderCopy(renderer,textureText,NULL,&paddle);

        
        //BALL RENDERING

        ball.texture.x = ball_x;
        hitbox_ball.texture.x = ball_x -1;

        if (ball_y >= 590) {
                fly_flag = false;
        }

        if (fly_flag) {
            if ( ball_x + ball.texture.w >= res_width - 20 || ball_x <= 20 || (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y + 10 < 550))) {
                ball.dir_x *= -1;
                hitbox_ball.dir_x *= -1;
            }
            

            else if ( ball_y + ball.texture.h >= res_height || ball_y <= 60 || (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y + 10 < 550))) {
                ball.dir_y *= -1;
                hitbox_ball.dir_y *= -1;
            }

            if (SDL_HasIntersection(&hitbox_ball.texture, &paddle) && (ball_y + 10> 550)) {
                printf("texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                if (SDL_HasIntersection(&hitbox_ball.texture, &paddle)) {
                    ball_y -= ((ball_y + ball.texture.h) - 550 + 2) ; 
                }
                printf("AFTER:6 texture_y: %d,ball_y:%d\n",hitbox_ball.texture.y, ball_y);
                ball.dir_x *= -1;
                hitbox_ball.dir_x *= -1;
                ball.dir_y *= -1;
                hitbox_ball.dir_y *= -1;
            }

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

    quit_game(&window,&renderer);

    return 0;
}