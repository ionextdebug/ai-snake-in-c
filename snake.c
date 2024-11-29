#include "snake.h"

#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700
#define GRID_SIZE 20
#define GRID_DIM 600

enum
{
    TRY_FORWARD,
    TRY_LEFT,
    TRY_RIGHT,
};

typedef struct
{
    int x;
    int y;
} apple;

apple Apple;

enum
{
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT
};

struct snake
{
    int x;
    int y;
    int dir;

    struct snake *next;
};

typedef struct snake Snake;

Snake *head;
Snake *tail;

void init_snake()
{
    Snake *new = malloc(sizeof(Snake));
    new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->dir = SNAKE_UP;
    new->next = NULL;

    head = new;
    tail = new;

    return;
}

void increase_snake()
{
    Snake *new = malloc(sizeof(Snake));

    switch (tail->dir)
    {
    case SNAKE_UP:
        new->x = tail->x;
        new->y = tail->y + 1;
        break;
    case SNAKE_DOWN:
        new->x = tail->x;
        new->y = tail->y - 1;
        break;
    case SNAKE_LEFT:
        new->x = tail->x + 1;
        new->y = tail->y;
        break;
    case SNAKE_RIGHT:
        new->x = tail->x - 1;
        new->y = tail->y;
        break;
    default:
        break;
    }

    new->x = tail->x;
    new->y = tail->y - 1;
    new->dir = tail->dir;

    new->next = NULL;
    tail->next = new;

    tail = new;

    return;
}

void move_snake()
{

    int prev_x = head->x;
    int prev_y = head->y;
    int prev_dir = head->dir;

    switch (head->dir)
    {
    case SNAKE_UP:
        head->y--;
        break;
    case SNAKE_DOWN:
        head->y++;
        break;
    case SNAKE_LEFT:
        head->x--;
        break;
    case SNAKE_RIGHT:
        head->x++;
        break;
    }

    Snake *track = head;

    if (track->next != NULL)
    {
        track = track->next;
    }

    while (track != NULL)
    {

        int save_x = track->x;
        int save_y = track->y;
        int save_dir = track->dir;

        track->x = prev_x;
        track->y = prev_y;
        track->dir = prev_dir;
        track = track->next;

        prev_x = save_x;
        prev_y = save_y;
        prev_dir = save_dir;
    }

    return;
}

void reset_snake()
{

    Snake *track = head;
    Snake *temp;

    while (track != NULL)
    {
        temp = track;
        track = track->next;
        free(temp);
    }

    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();

    return;
}

void render_snake(SDL_Renderer *renderer, int x, int y)
{
    int seg_size = GRID_DIM / GRID_SIZE;
    SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);
    SDL_Rect seg;
    seg.w = seg_size;
    seg.h = seg_size;

    Snake *track = head;

    while (track != NULL)
    {
        seg.x = x + track->x * seg_size;
        seg.y = y + track->y * seg_size;

        SDL_RenderFillRect(renderer, &seg);

        track = track->next;
    }

    return;
}

void render_grid(SDL_Renderer *renderer, int x, int y)
{

    SDL_Rect cell;
    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 255);

    int cell_size = GRID_DIM / GRID_SIZE;
    cell.w = cell_size;
    cell.h = cell_size;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    return;
}

void gen_apple()
{

    bool in_snake;
    do
    {

        in_snake = false;
        Apple.x = rand() % GRID_SIZE;
        Apple.y = rand() % GRID_SIZE;

        Snake *track = head;

        while (track != NULL)
        {
            if (track->x == Apple.x && track->y == Apple.y)
            {
                in_snake = true;
            }
            track = track->next;
        }

    } while (in_snake);
}

void render_apple(SDL_Renderer *renderer, int x, int y)
{
    int apple_size = GRID_DIM / GRID_SIZE;
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 255);
    SDL_Rect apple;
    apple.w = apple_size;
    apple.h = apple_size;
    apple.x = x + Apple.x * apple_size;
    apple.y = y + Apple.y * apple_size;

    SDL_RenderFillRect(renderer, &apple);
}

void detect_apple()
{
    if (head->x == Apple.x && head->y == Apple.y)
    {
        gen_apple();
        increase_snake();
    }

    return;
}

void detect_crash()
{
    if (head->x < 0 || head->x >= GRID_SIZE || head->y < 0 || head->y >= GRID_SIZE)
    {
        reset_snake();
    }

    Snake *track = head;

    if (track->next != NULL)
    {
        track = track->next;
    }

    while (track != NULL)
    {
        if (track->x == head->x && track->y == head->y)
        {
            reset_snake();
        }
        track = track->next;
    }

    return;
}

void turn_left()
{

    switch (head->dir)
    {
    case SNAKE_UP:
        head->dir = SNAKE_LEFT;
        break;
    case SNAKE_DOWN:
        head->dir = SNAKE_RIGHT;
        break;
    case SNAKE_LEFT:
        head->dir = SNAKE_DOWN;
        break;
    case SNAKE_RIGHT:
        head->dir = SNAKE_UP;
        break;
    }

    return;
}

void turn_right()
{
    switch (head->dir)
    {
    case SNAKE_UP:
        head->dir = SNAKE_RIGHT;
        break;
    case SNAKE_DOWN:
        head->dir = SNAKE_LEFT;
        break;
    case SNAKE_LEFT:
        head->dir = SNAKE_UP;
        break;
    case SNAKE_RIGHT:
        head->dir = SNAKE_DOWN;
        break;
    }

    return;
}

int state(int try)
{
    int reward = 0;

    int try_x = head->x;
    int try_y = head->y;

    switch (head->dir)
    {
    case SNAKE_UP:
        switch (try)
        {
        case TRY_FORWARD:
            try_y--;
            break;
        case TRY_LEFT:
            try_x--;
            break;
        case TRY_RIGHT:
            try_x++;
            break;
        }
        break;
    case SNAKE_DOWN:
        switch (try)
        {
        case TRY_FORWARD:
            try_y++;
            break;
        case TRY_LEFT:
            try_y++;
            break;
        case TRY_RIGHT:
            try_y--;
            break;
        }
        break;
    case SNAKE_LEFT:
        switch (try)
        {
        case TRY_FORWARD:
            try_x--;
            break;
        case TRY_LEFT:
            try_y++;
            break;
        case TRY_RIGHT:
            try_y--;
            break;
        }
        break;
    case SNAKE_RIGHT:
        switch (try)
        {
        case TRY_FORWARD:
            try_x++;
            break;
        case TRY_LEFT:
            try_y--;
            break;
        case TRY_RIGHT:
            try_y++;
            break;
        }
        break;
    }

    if (try_x < 0 || try_x > GRID_SIZE - 1)
    {
        reward += -100;
    }

    if (try_y < 0 || try_y > GRID_SIZE - 1)
    {
        reward += -100;
    }

    if (try_x == Apple.x && try_y == Apple.y)
    {
        reward += 100;
    }

    int diff_x = abs(head->x - Apple.x);
    int diff_y = abs(head->y - Apple.y);
    int try_diff_x = abs(try_x - Apple.x);
    int try_diff_y = abs(try_y - Apple.y);

    if (try_diff_x < diff_x)
    {
        reward += 5;
    }

    if (try_diff_y < diff_y)
    {
        reward += 5;
    }

    Snake *track = head;

    if (track->next != NULL)
    {
        track = track->next;
    }

    while (track != NULL)
    {
        if (try_x == track->x && try_y == track->y)
        {
            reward += -100;
        }
        track = track->next;
    }

    return reward;
}

void ai()
{

    int try_forward = state(TRY_FORWARD);
    int try_left = state(TRY_LEFT);
    int try_right = state(TRY_RIGHT);

    if (try_forward >= try_left && try_forward >= try_right)
    {
        // continue forward
    }
    else
    {
        if (try_left > try_right)
        {
            turn_left();
        }
        else
        {
            turn_right();
        }
    }

    return;
}

int main(void)
{

    srand(time(0));

    init_snake();
    increase_snake();
    increase_snake();
    increase_snake();

    gen_apple();

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool quit = false;
    int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIM / 2);
    int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIM / 2);

    if (SDL_INIT_VIDEO < 0)
    {
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
    }

    window = SDL_CreateWindow(
        "Snake",
        WINDOW_Y,
        WINDOW_X,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        fprintf(stderr, "ERROR: window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        fprintf(stderr, "ERROR: renderer");
    }

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYUP:
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    head->dir = SNAKE_UP;
                    break;
                case SDLK_DOWN:
                    head->dir = SNAKE_DOWN;
                    break;
                case SDLK_LEFT:
                    head->dir = SNAKE_LEFT;
                    break;
                case SDLK_RIGHT:
                    head->dir = SNAKE_RIGHT;
                    break;
                }
                break;
            }
        }

        SDL_RenderClear(renderer);

        move_snake();
        detect_apple();
        detect_crash();
        render_grid(renderer, grid_x, grid_y);
        render_snake(renderer, grid_x, grid_y);
        render_apple(renderer, grid_x, grid_y);

        ai();

        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);
        SDL_Delay(200);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}