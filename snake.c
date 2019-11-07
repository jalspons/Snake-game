#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "snake.h"


void loadGame(Game *game)
{
    Snake *snake = malloc(sizeof(Snake));
    snake->x = SCREEN_WIDTH / 2,       // X position
    snake->y = SCREEN_HEIGHT / 2,      // Y position
    snake->dir = DOWN;
    snake->next = NULL;
    snake->last = NULL;

    Food *food = malloc(sizeof(Food));
    food->x = SCREEN_WIDTH / 4;
    food->y = SCREEN_HEIGHT / 4;

    game->snake = snake;
    game->food = food;
}

int canSnakeEat(Game *game)
{
    Snake *snake = game->snake;
    Food *food = game->food;

    printf("snake_x: %d, snake_y: %d\n", snake->x, snake->y);
    printf("food_x: %d, food_y: %d\n", food->x, food->y);

    return (snake->x == food->x && snake->y == food->y);
}

void destroyGame(Game *game)
{
    for (Tail *tail = game->snake->next; tail != NULL; tail = tail->next)
    {
        free(tail);
    }
    free(game->snake);
    free(game->food);

    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
}

int moveSnake(Game *game)
{
    Snake *snake = game->snake;
    Food *food = game->food;

    int prev_x = snake->x, temp_x = 0;
    int prev_y = snake->y, temp_y = 0;
    if (snake->dir == UP) {
        snake->y = (snake->y - SIZE) % SCREEN_HEIGHT;
    } else if (snake->dir == DOWN) {
        snake->y = (snake->y + SIZE) % SCREEN_HEIGHT;
    } else if (snake->dir == LEFT) {
        snake->x = (snake->x - SIZE) % SCREEN_WIDTH;
    } else {
        snake->x = (snake->x + SIZE) % SCREEN_WIDTH;
    }

    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        if (snake->x == tail->x && snake->y == tail->y) {
            return 1;
        }
    }

    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        int temp_x = tail->x;
        int temp_y = tail->y;

        tail->x = prev_x;
        tail->y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;
    }

    int canEat = canSnakeEat(game);
    if (canEat) {
        Tail *new = malloc(sizeof(Tail));  
        new->x = prev_x;
        new->y = prev_y;
        new->next = NULL;

        if (snake->next == NULL) {
            snake->next = new;
            snake->last = new;
        } else {
            snake->last->next = new;
            snake->last = new;
        }

        game->food->x = (snake->x + 50) % SCREEN_WIDTH; 
        game->food->y = (snake->y + 50) % SCREEN_HEIGHT;
    }

    return 0;
}

int processEvents(Game *game)
{
    SDL_Event event;
    int done = 0;
    Snake *snake = game->snake;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(game->window)
                {
                    SDL_DestroyWindow(game->window);
                    game->window = NULL;
                }
                break;
            }
            case SDL_QUIT:
            {
                // Quit out of the game
                done = 1;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                    {
                        if (snake->dir != DOWN) {
                            snake->dir = UP;
                        }
                        break;
                    }
                    case SDLK_DOWN:
                    {
                        if (snake->dir != UP) {
                            snake->dir = DOWN;
                        }
                        break;
                    }
                    case SDLK_LEFT:
                    {
                        if (snake->dir != RIGHT) {
                            snake->dir = LEFT;
                        }
                        break;
                    }
                    case SDLK_RIGHT:
                    {
                        if (snake->dir != LEFT) {
                            snake->dir = RIGHT;
                        }
                        break;
                    }
                    case SDLK_ESCAPE:
                    {
                        done = 1;
                        break;
                    }
                }
            }
        }
    }
 /*   
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_RIGHT]) {
        if (snake->dir != LEFT) {
            snake->dir = RIGHT;
        }
    }
    if (state[SDL_SCANCODE_UP]) {
        
    }
    if (state[SDL_SCANCODE_LEFT]) {
        if (snake->dir != RIGHT) {
            snake->dir = LEFT;
        }
    }
    if (state[SDL_SCANCODE_DOWN]) {
        if (snake->dir != UP) {
            snake->dir = DOWN;
        }
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        done = 1;
    }
    */
    
    return done;
}

void doRender(Game *game)
{
    SDL_Renderer *renderer = game->renderer;

    // Render display
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    // Clear the Screen
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect snakeRect = { game->snake->x, game->snake->y, SIZE, SIZE };
    SDL_RenderFillRect(renderer, &snakeRect);
    
    for (Tail *tail = game->snake->next; tail != NULL; tail = tail->next)
    {
        SDL_Rect tailRect = { tail->x, tail->y, SIZE, SIZE };
        SDL_RenderFillRect(renderer, &tailRect);
    }

    SDL_Rect food = { game->food->x, game->food->y, SIZE, SIZE };
    SDL_RenderFillRect(renderer, &food);

    // We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer); 
}

int main( int argc, char *argv[] )
{
    Game game;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init( SDL_INIT_VIDEO );
    window = SDL_CreateWindow("Game Window",            // Window title
                            SDL_WINDOWPOS_UNDEFINED,    // Initial x position
                            SDL_WINDOWPOS_UNDEFINED,    // Initial y position
                            SCREEN_WIDTH,               // Width, in pixels
                            SCREEN_HEIGHT,              // Height, in pixels
                            0);                         // flags

    renderer = SDL_CreateRenderer(window, 
                                        -1, 
                                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    game.renderer = renderer;
    game.window = window;

    loadGame(&game);

    int done = 0;    
    // Event loop
    while(!done) 
    {
        // Check for events
        done = processEvents(&game);
        // printf("Are we done yet? - %s\n", done ? "yes" : "No");

        // Move snake
        done = done || moveSnake(&game);

        // Render
        doRender(&game);

        // Don't burn the CPU
        SDL_Delay(100);
    }
    
    destroyGame(&game);

    SDL_Quit();

    return 0;
}