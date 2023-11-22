#include <SDL2/SDL.h>  // Vložení hlavního hlavičkového souboru SDL
#include <stdbool.h>

int main()
{
    bool keyboard = true;
    SDL_Init(SDL_INIT_VIDEO);   // Inicializace SDL

    // Vytvoření okna
    SDL_Window* window = SDL_CreateWindow(
        "SDL experiments",  // Název
        100,                // Souřadnice x
        100,                // Souřadnice y
        800,                // Šířka
        600,                // Výška
        SDL_WINDOW_SHOWN    // Okno se má po vytvoření rovnou zobrazit
    );
    SDL_SetWindowTitle(window,"Breakout");
    // Vytvoření kreslítka
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Event event;
    bool quit = false;

    int mouse_x = 200;
    int mouse_y = 0;

    while (!quit)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `e`
        while (SDL_PollEvent(&event))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `quit` na `true`
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_MOUSEMOTION && !keyboard) {
                mouse_x =  event.motion.x;
                if (mouse_x-50 <= 50) mouse_x = 80;
                else if (mouse_x+50 >= 750) mouse_x = 720;
            }
            if (SDL_KEYDOWN && keyboard) {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    mouse_x -=  10;
                    if (mouse_x-50 <= 20) mouse_x = 80;
                    break;
                
                case SDLK_RIGHT:
                    mouse_x +=  10;
                    if (mouse_x+50 >= 780) mouse_x = 720;
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

        // Nastavení barvy na červenou
        SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);

        SDL_Rect rectToDraw = {mouse_x-50,550,100,20}; //x_start, y_start, width, height
        SDL_RenderFillRect(renderer, &rectToDraw);
         printf("mousex:%d\n",mouse_x);

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }
    // Uvolnění prostředků
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}