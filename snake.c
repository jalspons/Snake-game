#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake.h"


int loadFont(Game *game)
{
    //Open the font
    game->gFont = TTF_OpenFont(font, font_size);
    if( game->gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    return 0;
}

int setBlock(Game *game, int x, int y, uint8_t value)
{
    int nx = x / block_size;
    int ny = y / block_size;
    game->block_statuses[nx + (screen_width / block_size) * ny] = value;
    return value;
}

int getBlock(Game *game, int x, int y)
{
    int nx = x / block_size;
    int ny = y / block_size;
    return game->block_statuses[nx + (screen_width / block_size) * ny];
}

void loadGameObjects(Game *game)
{
    // Create snake
    Snake *snake = (Snake *)calloc(1, sizeof(Snake));
    snake->x = screen_width / 2,       // X position
    snake->y = screen_height / 2,      // Y position
    snake->dir = DOWN;
    snake->next = NULL;
    snake->last = NULL;

    // Create food
    Food *food = (Food *)calloc(1, sizeof(Food));
    food->x = screen_width / 4;
    food->y = screen_height / 4;

    // Reserve the board blocks for snake and food
    setBlock(game, snake->x, snake->y, SNAKE_BLOCK);
    setBlock(game, food->x, food->y, FOOD_BLOCK);

    game->snake = snake;
    game->food = food;
}

void destroyGameObjects(Game *game)
{
    // Destroy Snake
    for(Tail *tail = game->snake->next; tail != NULL; )
    {
        Tail *next = tail->next;
        free(tail);
        tail = next;
    }
    free(game->snake);

    // Destroy food
    free(game->food);

    // Reset blocks
    for (int y = 0; y < screen_height; y += block_size) 
    {
        for (int x = 0; x < screen_width; x += block_size)
        {
            uint8_t block = getBlock(game, x, y);
            if (block == SNAKE_BLOCK || block == FOOD_BLOCK
                    || (use_wall && block == WALL_BLOCK)) {
                setBlock(game, x, y, FREE_BLOCK);
            }
        }
    }
}

void restartGame(Game *game)
{
    destroyGameObjects(game);
    loadGameObjects(game);
}

void createBorder(Game *game)
{
    int nx = screen_width / block_size;
    int ny = screen_height / block_size;
    Wall *border = calloc(1, sizeof(Wall));
    // n = 2nx + 2ny - overlapping blocks on corners
    border->numOfLedges = 2 * nx + 2* ny - 4;
    border->wall = calloc(border->numOfLedges, sizeof(Ledge));

    for (unsigned int i = 0; i < nx; i++)  
    {
        // Top side
        border->wall[2*i].x = i * block_size;      
        border->wall[2*i].y = 0;
        // Bottom side
        border->wall[2*i + 1].x = i * block_size;  
        border->wall[2*i + 1].y = screen_height - block_size;
    }
    for (unsigned int i = 1; i < ny - 1; i++)
    {
        // Left side
        border->wall[2*nx + 2*i - 2].x = 0; 
        border->wall[2*nx + 2*i - 2].y = i * block_size;
        // Right side
        border->wall[2*nx + 2*i - 2 + 1].x = screen_width - block_size; 
        border->wall[2*nx + 2*i - 2 + 1].y = i * block_size;
    }
    game->border = border;
}

void destroyBorder(Game *game)
{
    free(game->border->wall);
    free(game->border);
}

// A function for setting border blocks
void setBorderBlocks(Game *game, uint8_t value)
{
    Ledge *wall = game->border->wall;
    for (int i = 0; i < game->border->numOfLedges; i++)
    {
        setBlock(game, wall[i].x, wall[i].y, value);
    }
    // Move food inside walls if walls are enabled 
    if (use_wall) {
        // X-coords 
        if (game->food->x == 0) {
            game->food->x += block_size;
        } else if (game->food->x == (screen_width - block_size)) {
            game->food->x -= block_size;
        }
        // Y-coords
        if (game->food->y == 0) {
            game->food->y += block_size;
        } else if (game->food->y == (screen_height - block_size)) {
            game->food->y -= block_size;
        }
    }
}

void createPauseMenu(Game *game)
{
    
    /*
    // Amount of buttons
    int nButtons = 3;
    // Allocate memory for menu structures
    Menu *menu = calloc(1, sizeof(Menu));
    SDL_MessageBoxData *mBox = calloc(1, sizeof(SDL_MessageBoxData));
    SDL_MessageBoxButtonData **buttons = calloc(MENUS, sizeof(SDL_MessageBoxButtonData**));
    
    // Button labels
    const char title[] = "Game Menu";
    const char message[] = "Play or Quit?";
    const char playText[] = "Play";
    const char continueText[] = "Continue";
    const char restartText[] = "Restart";
    const char cancelText[] = "Quit";
    const char toggleWallText[] = "Toggle Walls";

    buttons[PAUSE_MENU] = calloc(nButtons, sizeof(SDL_MessageBoxButtonData));
    buttons[START_MENU] = calloc(nButtons, sizeof(SDL_MessageBoxButtonData));
    buttons[GAME_OVER_MENU] = calloc(nButtons, sizeof(SDL_MessageBoxButtonData));
    
    // ------------- PAUSE MENU ------------
    // Play button
    buttons[PAUSE_MENU][0].buttonid = GAME_RUNNING;
    buttons[PAUSE_MENU][0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[PAUSE_MENU][0].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[PAUSE_MENU][0].text, continueText, LABEL_TEXTLEN);

    // Quit button
    buttons[PAUSE_MENU][1].buttonid = GAME_EXIT;
    buttons[PAUSE_MENU][1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
    buttons[PAUSE_MENU][1].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[PAUSE_MENU][1].text, cancelText, LABEL_TEXTLEN);
    
    // Toggle Wall button
    buttons[PAUSE_MENU][2].buttonid = GAME_TOGGLE_WALL;
    buttons[PAUSE_MENU][2].flags = 0;
    buttons[PAUSE_MENU][2].text = calloc(LABEL_TEXTLEN , 1);
    strncpy(buttons[PAUSE_MENU][2].text, toggleWallText, LABEL_TEXTLEN);

    // ------------ START MENU ------------------
    // Play button
    buttons[START_MENU][0].buttonid = GAME_RUNNING;
    buttons[START_MENU][0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[START_MENU][0].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[START_MENU][0].text, playText, LABEL_TEXTLEN);

    // Quit button
    memcpy(buttons[START_MENU] + 1, buttons[PAUSE_MENU] + 1, sizeof(SDL_MessageBoxButtonData));
    
    // Toggle Wall button
    memcpy(buttons[START_MENU] + 2, buttons[PAUSE_MENU] + 2, sizeof(SDL_MessageBoxButtonData));
    

    // ------------- GAME OVER MENU --------------------
    // Play button
    buttons[GAME_OVER_MENU][0].buttonid = GAME_RESTART;
    buttons[GAME_OVER_MENU][0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[GAME_OVER_MENU][0].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[GAME_OVER_MENU][0].text, restartText, LABEL_TEXTLEN);

    // Quit button
    memcpy(buttons[GAME_OVER_MENU] + 1, buttons[PAUSE_MENU] + 1, sizeof(SDL_MessageBoxButtonData));
    
    // Toggle Wall button
    memcpy(buttons[GAME_OVER_MENU] + 2, buttons[PAUSE_MENU] + 2, sizeof(SDL_MessageBoxButtonData));

    // Init menu
    mBox->window = game->window;
    mBox->title = calloc(LABEL_TEXTLEN, 1);
    strncpy(mBox->title, title, LABEL_TEXTLEN);
    mBox->message = calloc(LABEL_TEXTLEN, 1);
    strncpy(mBox->message, message, LABEL_TEXTLEN);
    mBox->numbuttons = nButtons;
    mBox->buttons = buttons[START_MENU];
    mBox->colorScheme = NULL;
    mBox->flags = SDL_MESSAGEBOX_INFORMATION;

    // Assign menu item into the game instance
    menu->mBox = mBox;
    menu->buttons = buttons;
    game->menu = menu;

    // Amount of buttons
    int nButtons = 3;
    // Allocate memory for menu structures
    SDL_MessageBoxData *menu = calloc(1, sizeof(SDL_MessageBoxData));
    SDL_MessageBoxButtonData *buttons = calloc(nButtons, sizeof(SDL_MessageBoxButtonData));
    Menu *pausemenu = calloc(1, sizeof(Menu));
    pausemenu->menuButtonColors = SDL_AllocPalette(NCOLORS);

    // Button labels
    const char title[] = "Game Menu";
    const char message[] = "Play or Quit?";
    const char continueText[] = "Continue";
    const char cancelText[] = "Quit";
    const char toggleWallText[] = "Toggle Walls";
    
    // ------------- PAUSE MENU ------------
    // Play button
    buttons[0].buttonid = GAME_RUNNING;
    buttons[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
    buttons[0].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[0].text, continueText, LABEL_TEXTLEN);

    // Quit button
    buttons[1].buttonid = GAME_EXIT;
    buttons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
    buttons[1].text = calloc(LABEL_TEXTLEN, 1);
    strncpy(buttons[1].text, cancelText, LABEL_TEXTLEN);
    
    // Toggle Wall button
    buttons[2].buttonid = GAME_TOGGLE_WALL;
    buttons[2].flags = 0;
    buttons[2].text = calloc(LABEL_TEXTLEN , 1);
    strncpy(buttons[2].text, toggleWallText, LABEL_TEXTLEN);

    // Init menu
    menu->window = game->window;
    menu->title = calloc(LABEL_TEXTLEN, 1);
    strncpy(menu->title, title, LABEL_TEXTLEN);
    menu->message = calloc(LABEL_TEXTLEN, 1);
    strncpy(menu->message, message, LABEL_TEXTLEN);
    menu->numbuttons = nButtons;
    menu->buttons = buttons;
    menu->colorScheme = NULL;
    menu->flags = SDL_MESSAGEBOX_INFORMATION;
    
    // Textures for menu buttons
    pausemenu->mBox = menu;
    pausemenu->buttons = buttons;
    pausemenu->menuState = PLAY;

    //printf("Color1 %d", pausemenu->menuButtonColors->colors[0].r);

    game->menu = pausemenu;
    */

    unsigned int n_items = sizeof(pause_menu_items) / sizeof(MenuItem);

    for (unsigned int i = 0; i < n_items; i++)
    {
        MenuItem *curr = &pause_menu_items[i % n_items];
        MenuItem *next = &pause_menu_items[(i + 1) % n_items];

        curr->next = next;
        next->prev = curr;
    }

    active_menu_item = &pause_menu_items[0];
}

void destroyGame(Game *game)
{
    // Free memory used by snake and food
    destroyGameObjects(game);

    // Free memory used by wall
    destroyBorder(game);

    // Free status blocks
    free(game->block_statuses);
}

int collision(int x1, int y1, int s1, int x2, int y2, int s2)
{
    return (x1 + s1 > x2 && x1 < x2 + s2 &&
            y1 + s1 > y2 && y1 < y2 + s2);
}

int collisionWithSnake(Game *game, int x, int y, int s)
{
    Snake *snake = game->snake;

    // Head collision
    if (collision(x, y, s, snake->x, snake->y, block_size)) 
    {
        return 1;
    }

    // Tail collision
    for (Tail *tail = snake->next; tail != NULL;  tail = tail->next)
    {
        if (collision(x, y, s, tail->x, tail->y, block_size))
        {
            return 1;
        }
    }
    return 0;
}

int getFreeBlocks(Game *game, int *x_list, int *y_list)
{    
    int i = 0;
    for (int y = 0; y < screen_height; y += block_size)
    {
        for (int x = 0; x < screen_width; x += block_size)
        {
            if (getBlock(game, x, y) == FREE_BLOCK) {
                x_list[i] = x;
                y_list[i] = y;
                //printf("Free found (%d, %d)\n", x_list[i], y_list[i]);
                i++;
            }
        }
    }
    return i;
}

int calculateDistance(int x0, int y0, int x1, int y1)
{
    return abs(x1 - x0) + abs(y1 - y0);
}

int replaceFood(Game *game)
{
    Food *food = game->food;
    Snake *snake = game->snake;
    Ledge *wall = game->border->wall;

    int x_list[(screen_width * screen_height) / (block_size * block_size)];
    int y_list[(screen_width * screen_height) / (block_size * block_size)];
    int n = getFreeBlocks(game, x_list, y_list);

    // Place new food at least 10 steps away
    int distance_threshold = 10 * block_size; 
    int best_distance = 0;
    int best = 0;

    unsigned int i = 0;
    while(i < n)
    {
        int j = random() % n;
        int d = calculateDistance(food->x, food->y, x_list[j], y_list[j]);
        if (d > best_distance)
        {
            best_distance = d;
            best = j;
        }
        if (d > distance_threshold)
        {
            break;
        }
        i++;
    }

    food->x = x_list[best];
    food->y = y_list[best];
    
    //printf("Food replaced to (%d,%d), d_threshold %d\n", food->x, food->y, distance_threshold);
    return 0;
}

void moveSnake(Game *game)
{
    Snake *snake = game->snake;
    Food *food = game->food;
    Ledge *wall = game->border->wall;

    int prev_x = snake->x, temp_x = 0;
    int prev_y = snake->y, temp_y = 0;

    // Move snake's head
    if (snake->dir == UP) {
        snake->y = (screen_height + snake->y - block_size) % screen_height;
    } else if (snake->dir == DOWN) {
        snake->y = (snake->y + block_size) % screen_height;
    } else if (snake->dir == LEFT) {
        snake->x = (screen_width + snake->x - block_size) % screen_width;
    } else {
        snake->x = (snake->x + block_size) % screen_width;
    }
    setBlock(game, snake->x, snake->y, SNAKE_BLOCK);

    // Check if head collides with tail
    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        int eatTail = collision(snake->x, snake->y, block_size, 
                                tail->x, tail->y, block_size);
        if (eatTail) {
            game->gameState = GAME_OVER;
        }
    }

    // Check if head collides with border
    if (use_wall) {
        for (unsigned int i = 0; i < game->border->numOfLedges; ++i)
        {
            int hitWall = collision(snake->x, snake->y, block_size, 
                                    wall[i].x, wall[i].y, block_size);
            if (hitWall) {
                game->gameState = GAME_OVER;
            }
        }
    }

    // Move each tail object by following the former objects
    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        if (tail->next == NULL) {
            setBlock(game, tail->x, tail->y, FREE_BLOCK);
        }
        
        int temp_x = tail->x;
        int temp_y = tail->y;

        tail->x = prev_x;
        tail->y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;
    }

    // Check can snake eat the food
    int canEat = collision(snake->x, snake->y, block_size, 
                            food->x, food->y, block_size);
    if (canEat) {
        // Add new tail at the end of the snake
        Tail *new = (Tail *)calloc(1, sizeof(Tail));  
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

        //food->x = (snake->x + 4 * block_size) % (screen_width - 2*block_size) + block_size; 
        //food->y = (snake->y + 4 * block_size) % (screen_height - 2*block_size) + block_size;
        if (replaceFood(game))
        {
            printf("Could not replace food. Exiting. \n");
            game->gameState = GAME_EXIT;
        }
    }
}

void processEvents(Game *game)
{
    SDL_Event event;
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
            game->gameState = GAME_EXIT;
            break;
        }
        case SDL_KEYDOWN:
        {
            switch(game->gameState)
            {
            case GAME_RUNNING:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                {
                    if (snake->dir != DOWN) {
                        snake->dir = UP;
                        return;
                    }
                    break;
                }
                case SDLK_DOWN:
                {
                    if (snake->dir != UP) {
                        snake->dir = DOWN;
                        return;
                    }
                    break;
                }
                case SDLK_LEFT:
                {
                    if (snake->dir != RIGHT) {
                        snake->dir = LEFT;
                        return;
                    }
                    break;
                }
                case SDLK_RIGHT:
                {
                    if (snake->dir != LEFT) {
                        snake->dir = RIGHT;
                        return;
                    }
                    break;
                }
                case SDLK_ESCAPE:
                {
                    game->gameState = GAME_PAUSED;
                    break;
                }
                } break;
            }
            case GAME_NEW:
            case GAME_OVER:
            case GAME_PAUSED:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                {
                    active_menu_item = active_menu_item->prev;
                    break;
                }
                case SDLK_DOWN:
                {
                    active_menu_item = active_menu_item->next;
                    break;
                }
                case SDLK_RETURN:
                {
                    switch(active_menu_item->id) {
                    case PLAY: 
                    {
                        if (game->gameState != GAME_PAUSED) {
                                restartGame(game);
                        }
                        game->gameState = GAME_RUNNING; 
                        break;
                    }
                    case QUIT:
                    {
                        game->gameState = GAME_EXIT; 
                        break;
                    } 
                    case USE_WALL: 
                    {
                        if (use_wall) {
                            use_wall = FALSE;
                            setBorderBlocks(game, FREE_BLOCK);
                        } else {
                            use_wall = TRUE;
                            setBorderBlocks(game, WALL_BLOCK);
                        }
                        break;
                    }
                    }
                }
                }
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
}

void renderMenu(Game *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
    uint16_t y_pos = menu_y_offset;
    int n_items = sizeof(pause_menu_items) / sizeof(MenuItem);

    for (unsigned int i = 0; i < n_items; i++ )
    {
        MenuItem *menu_item = &pause_menu_items[i];
        SDL_Surface *text_surface;
        int w, h;

        //printf("Rendering button %s\n", menu_item->text);

        if (active_menu_item->id == menu_item->id) {
            text_surface = TTF_RenderUTF8_Shaded(
                game->gFont, menu_item->text, colors[color_font], colors[color_active]);
        } else {
            text_surface = TTF_RenderUTF8_Solid(
                game->gFont, menu_item->text, colors[color_font]);
        }

        w = text_surface->w;
        h = text_surface->h;

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect text_rect = { 
            (screen_width / 2) - 5*(strlen(menu_item->text)), y_pos, w, h};

        SDL_RenderCopy(renderer, texture, NULL, &text_rect);

        SDL_FreeSurface(text_surface);

        y_pos += h + menu_y_spacing;
    }
}

void renderGame(Game *game)
{
    SDL_Renderer *renderer = game->renderer;
    
    // Draw wall with grey, if it is used
    if (use_wall) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
        Ledge *wall = game->border->wall;
        for (unsigned int i = 0; i < game->border->numOfLedges; ++i)
        {
            SDL_Rect ledgeRect = { wall[i].x, wall[i].y, block_size, block_size };
            SDL_RenderFillRect(renderer, &ledgeRect);
        }
    }

    // Draw snake with WHITE 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect snakeRect = { game->snake->x, game->snake->y, block_size, block_size };
    SDL_RenderFillRect(renderer, &snakeRect);
    for (Tail *tail = game->snake->next; tail != NULL; tail = tail->next)
    {
        SDL_Rect tailRect = { tail->x, tail->y, block_size, block_size };
        SDL_RenderFillRect(renderer, &tailRect);
    }

    // Draw food with blue
    SDL_SetRenderDrawColor(renderer, 0, 50, 120, 0);
    SDL_Rect food = { game->food->x, game->food->y, block_size, block_size };
    SDL_RenderFillRect(renderer, &food);
}

void doRender(Game *game)
{
    SDL_Renderer *renderer = game->renderer;

    // Render display
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    // Clear the Screen
    SDL_RenderClear(renderer);

    // Render game or menu
    switch(game->gameState)
    {
    case GAME_PAUSED:
    case GAME_OVER:
    case GAME_NEW:
    {
        renderMenu(game);
        break;
    }
    default:
    {
        renderGame(game);
        break;
    }
    }

    // We are done drawing, "present" or show to the screen what we've drawn
    SDL_RenderPresent(renderer); 
}

int main( int argc, char *argv[] )
{
    Game game;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init( SDL_INIT_VIDEO );
    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        game.gameState = GAME_EXIT;
    }

    window = SDL_CreateWindow("Game Window",            // Window title
                            SDL_WINDOWPOS_UNDEFINED,    // Initial x position
                            SDL_WINDOWPOS_UNDEFINED,    // Initial y position
                            screen_width,               // Width, in pixels
                            screen_height,              // Height, in pixels
                            0);                         // flags

    renderer = SDL_CreateRenderer(window, 
                                -1, 
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    // Init random number generator
    srand(time(NULL));

    game.renderer = renderer;
    game.window = window;
    game.gameState = GAME_NEW;
    game.block_statuses = malloc(
        (screen_width * screen_height) / (block_size * block_size));
    memset(game.block_statuses, FREE_BLOCK, 
        (screen_width * screen_height) / (block_size * block_size));

    loadFont(&game);
    createPauseMenu(&game);
    loadGameObjects(&game);
    createBorder(&game);

    game.game_speed = 100 / 1;

    // Event loop
    while(game.gameState != GAME_EXIT) 
    {
        // Check for events
        processEvents(&game);

        // Move snake
        if (game.gameState == GAME_RUNNING) {
            moveSnake(&game);
        }

        // Render
        doRender(&game);

        // The time for the game step
        SDL_Delay(game.game_speed);
    }
    
    destroyGame(&game);

    //Free global font
    TTF_CloseFont( game.gFont );

    // Destroy window and renderer
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();
    SDL_Quit();

    return 0;
}