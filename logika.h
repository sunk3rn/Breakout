#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

void game_init(SDL_Window ** window, SDL_Renderer ** renderer, int width, int height);

int calc_digit(int num); //Vypočítá počet cifer

char * format_score(int number); //Vrací ukazatel na formátované skóre v podobě osmimístného stringu

char * format_lives(int lives); //Vrací ukazatel na formátované životy

void quit_game(SDL_Window ** window, SDL_Renderer ** renderer);

void read_score(int * score_list); // Přečtení skóre ze souboru

void save_score(int * score_list, int score); //Uložení skóre do souboru