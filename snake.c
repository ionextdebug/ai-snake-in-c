#include "snake.h"

#define WINDOW_X 10
#define WINDOW_Y -20
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define GRID_SIZE 20
#define GRID_DIM 600

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

int main(void)
{

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    bool quit = false;

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
                }
                break;
            }
        }

        SDL_RenderClear(renderer);

        render_grid(renderer, 20, 20);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}