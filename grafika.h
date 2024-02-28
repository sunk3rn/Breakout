#include <SDL2/SDL.h> 
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect texture;
    float dir_x;
    float dir_y;
} Ball;

typedef struct {
    SDL_Rect texture;
    bool broken;
    int health;
} Block;

Block * generate_blocks(int amount, int health, int row );

void draw_blocks(SDL_Renderer * renderer, Block * blocks, int amount);

void draw_bounds(SDL_Renderer * renderer,int res_width, int res_height);

void check_row_collision(Ball *ball, Ball hitbox_ball, Block * blocks,int amount, int * score);

void draw_gui(SDL_Renderer * renderer,TTF_Font* font,int score,int lives);

void draw_logo(SDL_Renderer * renderer,TTF_Font* font,int posx, int posy, int width, int height);

void draw_prompt(SDL_Renderer * renderer,TTF_Font* font, char * text,int posx, int posy, int width, int height, SDL_Color textColor);

void draw_menu(SDL_Renderer * renderer,TTF_Font* font,int menu, int menu_option,SDL_Color textColor,SDL_Color selectedColor,int lives, bool keyboard,int * score);