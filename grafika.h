#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect texture;
    int dir_x;
    int dir_y;
} Ball;

typedef struct {
    SDL_Rect texture;
    bool broken;
    int health;
} Block;

Block * generate_blocks(
    int amount, // Počet bloků k vygenerování
    int health, // počet životů těchto bloků
    int row // řádek, v kterém se mají zobrazit
);

void draw_blocks(SDL_Renderer * renderer, Block * blocks, int amount);

void draw_bounds(SDL_Renderer * renderer,int res_width, int res_height);

void check_row_collision(Ball *ball, Ball hitbox_ball, Block * blocks,int amount, int * score);

void draw_gui(SDL_Renderer * renderer,TTF_Font* font,int score,int lives);
