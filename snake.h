#ifndef SNAKE_H_
#define SNAKE_H_

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SIZE 20
#define LABEL_TEXTLEN 20

#define MENU_FONT "DroidSans.ttf"
#define MENU_FONT_SIZE 15
#define FONT_OUTLINE 30

// Colors
#define NCOLORS 4
#define COLOR_FONT {250, 250, 250, 0}
#define COLOR_ACTIVE {200, 200, 200, 100}
#define COLOR_INACTIVE {100, 100, 100, 100}
#define COLOR_BG {25,25,25,100}


#define MIN(x,y) ((x < y) ? x : y)
#define MAX(x,y) ((x < y) ? y : x)

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

    GAME_RESTART,
    GAME_TOGGLE_WALL
};

enum menuStatus {
    PLAY,
    QUIT,
    TOGGLE_WALL,
    STATES_LENGTH
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
    SDL_MessageBoxButtonData *buttons;
    enum menuStatus menuState;
    SDL_Palette *menuButtonColors;
} Menu;

typedef struct {
    Snake *snake;
    Food *food;
    Wall *border;
    TTF_Font *gFont;
    SDL_Renderer *renderer;
    SDL_Window *window;
    Menu *menu;
    enum gameStatus gameState;
    int exitStatus, useWall, gamespeed;
} Game;

#endif