#ifndef SNAKE_H_
#define SNAKE_H_


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SIZE 20
#define LABEL_TEXTLEN 20


enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum gameStatus {
    GAME_OVER,
    GAME_PAUSED,
    GAME_NEW,
    GAME_RUNNING,
    GAME_EXIT,

    GAME_TOGGLE_WALL,
    GAME_RESTART
};

enum menuStatus {
    PLAY,
    QUIT,
    TOGGLE_WALL,
    MENU_STATUSES
};

enum menuButtonAction {
    PAUSE_MENU,
    START_MENU,
    GAME_OVER_MENU
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
    Ledge *wall;
    int numOfLedges;
} Wall;

typedef struct {
    SDL_MessageBoxData *mBox;
    SDL_MessageBoxButtonData **buttons;
} Menu;

typedef struct {
    Snake *snake;
    Food *food;
    Wall *border;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_MessageBoxData *pauseMenu;
    enum gameStatus gameState;
    enum menuStatus menuState;
    int exitStatus, useWall;
} Game;

#endif