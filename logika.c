#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

void game_init (SDL_Window ** window, SDL_Renderer ** renderer, int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);   // Inicializace SDL	

    if(TTF_Init() == -1) {
        printf( "Unable to render text surface! SDL2_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }
    else {
        printf("SDL2_ttf working\n");
    }

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

int calc_digit(int num) {
    int digits = 0;
    if (num == 0) {
        return 1;
    }
    while (num > 0) {
        num /= 10;
        digits++;
    }

    return digits;
}

char * format_score(int number) {
    int digits = calc_digit(number);
    char zeroes[9] = "0000000";
    char *output = (char *)malloc(9 + 1);
    zeroes[8-digits] = '\0';
    sprintf(output,"%s%d",zeroes,number);
    return output;
}

char * format_lives(int lives) {
    char *output = (char *)malloc(9 + 1);
    sprintf(output,"Lives: %d",lives);
    return output;
}

void quit_game(SDL_Window ** window, SDL_Renderer ** renderer) {
    // Uvolnění prostředků
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    SDL_Quit();
    TTF_Quit();
}

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void sortThreeInts(int* a, int* b, int* c) {
    if (*a < *b) {
        swap(a, b);
    }

    if (*b < *c) {
        swap(b, c);
    }

    if (*a < *b) {
        swap(a, b);
    }
}

void read_score(int * score_list) {
    FILE * score_file = NULL;
    score_file = fopen("../score.txt", "rt");
    char line[100];
    for (int i = 0; i < 3; i++) {
        if (fgets(line, sizeof(line), score_file) != NULL) {
            score_list[i] = atoi(line);
            //printf("Score[%d]: %d\n", i + 1, score_list[i]);
        } else {
            printf("Error čtení skóre\n");
            break;
        }
    }

    sortThreeInts(&score_list[0],&score_list[1],&score_list[2]);
    fclose(score_file);
}

void save_score(int * score_list, int score) {
    //Uložení skóre
    for (int i = 2; i >= 0;i--) {
        if (score_list[i] < score) {
            if (i != 2) {
                score_list[i+1] = score_list[i];
            }
            score_list[i] = score;  
        }
    }

    FILE * score_file = NULL;
    score_file = fopen("../score.txt", "wt");
    
    //Zapsání skóre do souboru
    for (int i = 0; i < 3; i++) {
        fprintf(score_file, "%d\n", score_list[i]);
    }
    fclose(score_file);
}

