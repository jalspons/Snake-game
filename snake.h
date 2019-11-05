#ifndef SNAKE_H_
#define SNAKE_H_


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SIZE 10

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Tail {
    int x, y;
    struct Tail *next;
};
typedef struct Tail Tail;

typedef struct {
    int x, y;
} Food;

typedef struct {
    int x, y;
    enum direction dir;
    Tail *next, *last;
} Snake;

typedef struct {
    int x, y;
} Ledge;

typedef struct {
    Snake *snake;
    Food *food;
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;

#endif