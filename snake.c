#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "snake.h"


int loadFont(Game *game)
{
    //Open the font
    game->gFont = TTF_OpenFont(MENU_FONT, MENU_FONT_SIZE);
    if( game->gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        return -1;
    }
    
    return 0;
}

void loadGameObjects(Game *game)
{
    // Create snake
    Snake *snake = (Snake *)calloc(1, sizeof(Snake));
    snake->x = SCREEN_WIDTH / 2,       // X position
    snake->y = SCREEN_HEIGHT / 2,      // Y position
    snake->dir = DOWN;
    snake->next = NULL;
    snake->last = NULL;

    // Create food
    Food *food = (Food *)calloc(1, sizeof(Food));
    food->x = SCREEN_WIDTH / 4;
    food->y = SCREEN_HEIGHT / 4;

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
}

void restartGame(Game *game)
{
    destroyGameObjects(game);
    loadGameObjects(game);
}

void createBorder(Game *game)
{
    Wall *border = calloc(1, sizeof(Wall));
    border->numOfLedges = 2 * SCREEN_WIDTH / SIZE + 2* SCREEN_HEIGHT / SIZE;
    border->wall = calloc(border->numOfLedges, sizeof(Ledge));

    int nx = SCREEN_WIDTH / SIZE;
    for (unsigned int i = 0; i < nx; ++i)  
    {
        // Top side
        border->wall[2*i].x = i * SIZE;      
        border->wall[2*i].y = 0;
        // Bottom side
        border->wall[2*i + 1].x = i * SIZE;  
        border->wall[2*i + 1].y = SCREEN_HEIGHT - SIZE;
    }

    int ny = SCREEN_HEIGHT / SIZE;
    for (unsigned int i = 0; i < ny; ++i)
    {
        // Left side
        border->wall[2*nx + 2*i].x = 0; 
        border->wall[2*nx + 2*i].y = i * SIZE;
        // Right side
        border->wall[2*nx + 2*i + 1].x = SCREEN_WIDTH - SIZE; 
        border->wall[2*nx + 2*i + 1].y = i * SIZE;
    }

    game->border = border;
}

void destroyBorder(Game *game)
{
    free(game->border->wall);
    free(game->border);
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
    game->menu = menu;*/

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

    SDL_Color colors[NCOLORS] = {
        COLOR_FONT,
        COLOR_ACTIVE,
        COLOR_INACTIVE,
        COLOR_BG
    };

    SDL_SetPaletteColors(pausemenu->menuButtonColors,
    colors, 0, NCOLORS);
    
    //printf("Color1 %d", pausemenu->menuButtonColors->colors[0].r);

    game->menu = pausemenu;
}

void destroyPauseMenu(Game *game)
{
    Menu *menu = game->menu;
    
    free(menu->mBox->title);
    free(menu->mBox->message);
    free(menu->buttons[0].text);
    free(menu->buttons[1].text);
    free(menu->buttons[2].text);
    free(menu->buttons);
    free(menu);
}

void destroyGame(Game *game)
{
    // Free memory used by snake and food
    destroyGameObjects(game);

    // Free memory used by wall
    destroyBorder(game);
    
    // Free memory used by menu
    destroyPauseMenu(game);
}

int collision(int x1, int y1, int s1, int x2, int y2, int s2)
{
    return (x1 + s1 > x2 && x1 < x2 + s2 &&
            y1 + s1 > y2 && y1 < y2 + s2);
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
        snake->y = (SCREEN_HEIGHT + snake->y - SIZE) % SCREEN_HEIGHT;
    } else if (snake->dir == DOWN) {
        snake->y = (snake->y + SIZE) % SCREEN_HEIGHT;
    } else if (snake->dir == LEFT) {
        snake->x = (SCREEN_WIDTH + snake->x - SIZE) % SCREEN_WIDTH;
    } else {
        snake->x = (snake->x + SIZE) % SCREEN_WIDTH;
    }

    // Check if head collides with tail
    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        int eatTail = collision(snake->x, snake->y, SIZE, 
                                tail->x, tail->y, SIZE);
        if (eatTail) {
            game->gameState = GAME_OVER;
        }
    }

    // Check if head collides with border
    if (game->useWall) {
        for (unsigned int i = 0; i < game->border->numOfLedges; ++i)
        {
            int hitWall = collision(snake->x, snake->y, SIZE, 
                                    wall[i].x, wall[i].y, SIZE);
            if (hitWall) {
                game->gameState = GAME_OVER;
            }
        }
    }


    // Move each tail object by following the former objects
    for (Tail *tail = snake->next; tail != NULL; tail = tail->next)
    {
        int temp_x = tail->x;
        int temp_y = tail->y;

        tail->x = prev_x;
        tail->y = prev_y;

        prev_x = temp_x;
        prev_y = temp_y;
    }

    // Check can snake eat the food
    int canEat = collision(snake->x, snake->y, SIZE, 
                            food->x, food->y, SIZE);
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

        food->x = (snake->x + 4 * SIZE) % (SCREEN_WIDTH - 2*SIZE) + SIZE; 
        food->y = (snake->y + 4 * SIZE) % (SCREEN_HEIGHT - 2*SIZE) + SIZE;
    }

}

void processEvents(Game *game)
{
    SDL_Event event;
    Snake *snake = game->snake;
    Menu *menu = game->menu;
    
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
                }
            }
            case GAME_NEW:
            case GAME_OVER:
            case GAME_PAUSED:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                {
                    // If the menu goes below bounds, return to bottom
                    menu->menuState = (STATES_LENGTH + (menu->menuState - 1)) % STATES_LENGTH;
                    break;
                }
                case SDLK_DOWN:
                {
                    // If the menu goes above bounds, return to top
                    menu->menuState = (menu->menuState + 1) % STATES_LENGTH;
                    break;
                }
                case SDLK_RETURN:
                {
                    switch(menu->menuState)
                    {
                    case PLAY:
                    {
                        if (game->gameState == GAME_OVER) {
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
                    case TOGGLE_WALL:
                    {
                        game->useWall = !game->useWall;
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

SDL_Surface *createTextSurface(Game *game, const char *str, enum menuStatus status)
{
    SDL_Color *colors = game->menu->menuButtonColors->colors;

    if (status == game->menu->menuState)
    {
        return TTF_RenderText_Shaded(game->gFont, str, colors[0], colors[1]);
    } else {
        return TTF_RenderText_Shaded(game->gFont, str, colors[0], colors[2]);
    }
}


SDL_Texture *createMenuTextures(Game *game, const char **str)
{
    if (game->gameState)
    {
        
    }
}

void renderMenu(Game *game)
{
    SDL_Renderer *renderer = game->renderer;
    int margin_bottom = 80;
    int spacing = 40;
    Menu *menu = game->menu;

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);

    SDL_Surface *playButtonSurface = createTextSurface(game, "PLAY", PLAY);
    SDL_Surface *quitButtonSurface = createTextSurface(game, "QUIT", QUIT);
    SDL_Surface *wallButtonSurface = createTextSurface(game, "TOGGLE WALL", TOGGLE_WALL);

    int w = MAX(playButtonSurface->w, MAX(quitButtonSurface->w, wallButtonSurface->w));
    int h = MAX(playButtonSurface->h, MAX(quitButtonSurface->h, wallButtonSurface->h));

    SDL_Texture *playButtonTexture = SDL_CreateTextureFromSurface(game->renderer, playButtonSurface);
    SDL_Texture *quitButtonTexture = SDL_CreateTextureFromSurface(game->renderer, quitButtonSurface);
    SDL_Texture *wallButtonTexture = SDL_CreateTextureFromSurface(game->renderer, wallButtonSurface);

    // Play button
    unsigned int y_pos = margin_bottom;
    SDL_Rect playBox = { (SCREEN_WIDTH - w) / 2, y_pos, w, h };
    SDL_RenderCopy(renderer, playButtonTexture, NULL, &playBox);

    // Quit Button
    y_pos += h + spacing;
    SDL_Rect quitBox = { (SCREEN_WIDTH - w) / 2, y_pos , w, h };
    SDL_RenderCopy(renderer, quitButtonTexture, NULL, &quitBox);

    // Toggle Wall button
    y_pos += h + spacing;
    SDL_Rect wallBox = { (SCREEN_WIDTH - w) / 2, y_pos , w, h };
    SDL_RenderCopy(renderer, wallButtonTexture, NULL, &wallBox);

    SDL_FreeSurface(playButtonSurface);
    SDL_FreeSurface(quitButtonSurface);
    SDL_FreeSurface(wallButtonSurface);
}

void renderGame(Game *game)
{
    SDL_Renderer *renderer = game->renderer;
    
    // Draw wall with grey, if it is used
    if (game->useWall) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 0);
        Ledge *wall = game->border->wall;
        for (unsigned int i = 0; i < game->border->numOfLedges; ++i)
        {
            SDL_Rect ledgeRect = { wall[i].x, wall[i].y, SIZE, SIZE };
            SDL_RenderFillRect(renderer, &ledgeRect);
        }
    }

    // Draw snake with WHITE 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect snakeRect = { game->snake->x, game->snake->y, SIZE, SIZE };
    SDL_RenderFillRect(renderer, &snakeRect);
    for (Tail *tail = game->snake->next; tail != NULL; tail = tail->next)
    {
        SDL_Rect tailRect = { tail->x, tail->y, SIZE, SIZE };
        SDL_RenderFillRect(renderer, &tailRect);
    }

    // Draw food with blue
    SDL_SetRenderDrawColor(renderer, 0, 50, 120, 0);
    SDL_Rect food = { game->food->x, game->food->y, SIZE, SIZE };
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
                            SCREEN_WIDTH,               // Width, in pixels
                            SCREEN_HEIGHT,              // Height, in pixels
                            0);                         // flags

    renderer = SDL_CreateRenderer(window, 
                                -1, 
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

    
    game.renderer = renderer;
    game.window = window;
    game.gameState = GAME_NEW;
    game.exitStatus = 0;
    game.useWall = 0;

    loadFont(&game);
    createPauseMenu(&game);
    loadGameObjects(&game);
    createBorder(&game);
    loadGameObjects(&game);

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
        SDL_Delay(100);
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