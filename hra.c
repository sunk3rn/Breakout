#include <SDL2/SDL.h>  // Vložení hlavního hlavičkového souboru SDL
#include <stdbool.h>

int main()
{
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
// Vytvoření kreslítka
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_Event event;
    bool quit = false;
    int pos = 100;
    
    int mouse_x = 0;
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
            if(event.type == SDL_MOUSEMOTION) {
                mouse_x =  event.motion.x;
                if (mouse_x-50 <= 50) mouse_x = 80;
                else if (mouse_x+50 >= 750) mouse_x = 720;
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

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }
    // Uvolnění prostředků
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}