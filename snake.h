#ifndef SNAKE_H_
#define SNAKE_H_

#include <SDL2/SDL_ttf.h>

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x < y) ? y : x)

enum { FALSE = 0, TRUE = 1 };

//----------------------------------------
// GAME
//----------------------------------------

enum direction { UP, DOWN, LEFT, RIGHT };

enum blockStatus { FREE_BLOCK, SNAKE_BLOCK, FOOD_BLOCK, WALL_BLOCK };

typedef struct Tail {
  int x;
  int y;
  struct Tail *next;
} Tail;

typedef struct {
  int x;
  int y;
} Food;

typedef struct {
  int x;
  int y;
  enum direction dir;
  Tail *next;
  Tail *last;
} Snake;

typedef struct {
  int x;
  int y;
} Ledge;

typedef struct {
  Ledge *wall;
  int numOfLedges;
} Wall;

enum gameStatus {
  GAME_OVER = 1,
  GAME_PAUSED,
  GAME_NEW,
  GAME_RUNNING,
  GAME_EXIT,
};

typedef struct {
  int window_width;
  int window_height;
  Snake *snake;
  Food *food;
  Wall *border;
  TTF_Font *gFont;
  // variable for holding statuses for all free blocks
  uint8_t *block_statuses;
  SDL_Renderer *renderer;
  SDL_Window *window;
  enum gameStatus gameState;
  int game_speed;
} Game;

#endif
