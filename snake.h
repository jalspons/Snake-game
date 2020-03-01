#ifndef SNAKE_H_
#define SNAKE_H_

#define extern
#define INIT(...) __VA_ARGS__

#define MIN(x,y) ((x < y) ? x : y)
#define MAX(x,y) ((x < y) ? y : x)

//-------------------------------------
// SETTINGS
//-------------------------------------

extern uint8_t use_wall;

int screen_width INIT(= 640);
int screen_height INIT(= 400);

int block_size INIT(= 20);

char *font INIT(= "DroidSans.ttf");
int font_size INIT(= 15);

//-------------------------------------
// COLORS
//-------------------------------------

static SDL_Color colors[] INIT(= {
    {250, 250, 250, 0},     // Font color
    {200, 200, 200, 100},   // Active color
    {25, 25, 25, 100}       
});

enum color_code {
    color_font = 0,
    color_active,
    color_bg
};

//--------------------------------
// MENU
//--------------------------------

static uint16_t menu_y_offset INIT(= 80);
static uint16_t menu_y_spacing INIT(= 40);

enum menu_id {
    PLAY,
    USE_WALL,
    QUIT
};

typedef struct MenuItem MenuItem;
struct MenuItem {
    int id;
    void *item;
    char *text;
    MenuItem *next;
    MenuItem *prev;
};

static MenuItem pause_menu_items[] INIT(= {
    {.id = PLAY, .text = "PLAY"},
    {.id = USE_WALL, .text = "TOGGLE WALL", .item = &use_wall},
    {.id = QUIT, .text= "QUIT GAME"}
});

extern MenuItem *active_menu_item;


//----------------------------------------
// GAME
//----------------------------------------

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

typedef struct Tail Tail;
struct Tail {
    int x, y;
    Tail *next;
};


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
    Snake *snake;
    Food *food;
    Wall *border;
    TTF_Font *gFont;
    SDL_Renderer *renderer;
    SDL_Window *window;
    enum gameStatus gameState;
    int exitStatus, useWall, gamespeed;
} Game;

#endif