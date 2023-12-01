#include <SDL2/SDL.h>  // Vložení hlavního hlavičkového souboru SDL
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

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
    }
    else {
        printf("SDL2_ttf working fine");
    }

    TTF_Font* ourFont = TTF_OpenFont("../assets/VT323-Regular.ttf",32);
    if(ourFont == NULL) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }
    SDL_Color textColor = {255,255,255,0};
    SDL_Surface* surfaceText = TTF_RenderText_Solid( ourFont, "hello world", textColor );
    SDL_Surface* surfaceScore = TTF_RenderText_Solid( ourFont, "0000000", textColor );

    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer,surfaceText);  
    SDL_Texture* scoreText = SDL_CreateTextureFromSurface(renderer,surfaceScore);  
    SDL_FreeSurface(surfaceText);
    SDL_FreeSurface(surfaceScore);

    SDL_Event event;
    bool quit = false;
    bool keyboard = true;
    int pos_x = 400;
    bool movement = false;
    const OUTERSPACE = rgb(68,74,67);

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

        // Vykreslení hráčské pálky
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
        SDL_Rect rectToDraw = {pos_x-50,550,100,20}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &rectToDraw);
        SDL_RenderCopy(renderer,textureText,NULL,&rectToDraw);

        //Vykreslení skóre
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_Rect scoreBox = {20,0,100,30};
        SDL_RenderFillRect(renderer,&scoreBox);
        SDL_RenderCopy(renderer,scoreText,NULL,&scoreBox);

        //Vykreslení okraje herního pole
        SDL_SetRenderDrawColor(renderer, 68, 74, 67, 255);
        SDL_Rect sideBoundL = {0,0,20,res_height}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &sideBoundL);
        SDL_Rect sideBoundR = {780,0,20,res_height};
        SDL_RenderFillRect(renderer, &sideBoundR);
        SDL_Rect topBound = {20,40,res_width - 40,20};
        SDL_RenderFillRect(renderer,&topBound);

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }

    quit_game(&window,&renderer);

    return 0;
}