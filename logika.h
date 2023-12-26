#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

void game_init (SDL_Window ** window, SDL_Renderer ** renderer, int width, int height);

int calc_digit(int num); //Vypočítá počet cifer

char * format_score(int number); //Vrací formátované skóre v podobě osmimístného stringu

char * format_lives(int lives);

void quit_game(SDL_Window ** window, SDL_Renderer ** renderer);
