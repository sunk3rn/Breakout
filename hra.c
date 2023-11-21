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
    SDL_Event e;
    bool quit = false;
    int pos = 100;

    while (!quit)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `e`
        while (SDL_PollEvent(&e))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `quit` na `true`
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Nastavení barvy vykreslování na černou
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Vykreslení pozadí
        SDL_RenderClear(renderer);

        // Nastavení barvy na červenou
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Vykreslení čáry
        SDL_RenderDrawLine(renderer, pos, pos, pos + 100, pos + 100);

        pos++;

        // Zobrazení vykreslených prvků na obrazovku
        SDL_RenderPresent(renderer);
    }
    // Uvolnění prostředků
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}



