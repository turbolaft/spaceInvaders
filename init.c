#include "common.h"
#include "structs.h"
#include "defs.h"

void initSDL() {
    int rendererFlags = SDL_RENDERER_ACCELERATED, windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL - %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                            SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window) {
        printf("Failed to open window - %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer) {
        printf("Failed to create renderer - %s\n", SDL_GetError());
        exit(1);
    }

    TTF_Init();
}

long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

void freeingAllocations(Entity* allocation) {
    while(allocation) {
        Entity* freee = allocation;
        if (allocation->surface != 0) SDL_FreeSurface(allocation->surface);
        allocation = allocation->next;
        free(freee);
    }
}