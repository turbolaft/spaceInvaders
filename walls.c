#include "common.h"
#include "structs.h"
#include "defs.h"
#include "draw.h"

Entity* gettingWall(int x, int y) {
    Entity* wall = NULL;

    wall = (Entity*) malloc(sizeof(Entity));
    memset(wall, 0, sizeof(Entity));

    wall->surface = loadSurface("resources/wall.png");
    wall->texture = SDL_CreateTextureFromSurface(app.renderer, wall->surface);
    if (!wall->texture)
    {
        printf("Error creating texture - %s\n", SDL_GetError());
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    wall->x = x;
    wall->y = y;
    wall->health = 1;
    wall->next = NULL;
    wall->width = 100;
    wall->height = 70;

    return wall;
}

void spawningWalls(Entity** listOfWalls) {
    Entity* t = NULL;

    for (int i = 0; i < 4; i++) {
        const int center = SCREEN_WIDTH / 2 - 350 + (100 * i) + (100 * i);
        Entity* wall = gettingWall(center, 450);

        if (i == 0) {
            *listOfWalls = wall;
            t = *listOfWalls;
        } else {
            t->next = wall;
            t = t->next;
        }
    }
}

void drawingWalls(Entity* listOfWalls) {
    for (Entity* t = listOfWalls; t != NULL; t = t->next) {
        blit(t->texture, t->x, t->y, t->width, t->height);
    }
}

SDL_bool checkOnWallTouch(Entity* listOfWalls, Entity* bullet, int isBoom) {
    for (Entity* t = listOfWalls; t != NULL; t = t->next) {
        const SDL_Rect wall = {t->x, t->y, t->width, t->height};

        for (int i = 0; i < bullet->height; i++) {
            for (int j = 0; j < bullet->width; j++) {
                const SDL_Point bulletPoint = {bullet->x + j, bullet->y + i};
                SDL_LockSurface(t->surface);
                uint8_t* pixels = (uint8_t*) t->surface->pixels;
                int pixel = (bulletPoint.y - t->y) * t->surface->w + (bulletPoint.x + t->x);

                if (SDL_PointInRect(&bulletPoint, &wall) == SDL_TRUE 
                        && !(pixels[pixel] == 0 && pixels[pixel + 1] == 0 && pixels[pixel + 2] == 0)) {
                    // printf("x - %d, y - %d\n", pixel % screen->pitch, pixel / screen->pitch);

                    if (isBoom) {
                        t->health = 0;
                        t->dy = bulletPoint.y - wall.y;
                        t->dx = bulletPoint.x - wall.x;
                    }

                    bullet->health = 0;

                    SDL_UnlockSurface(t->surface);
                    return SDL_TRUE;
                }
                SDL_UnlockSurface(t->surface);
            }
        }
    }
    
    return SDL_FALSE;
}

SDL_bool boom(int x, int y, float force, int depth, Entity* wall) {
    depth--;
    if (depth < 0) return SDL_TRUE;

    force -= fmod((float)rand(), 100) / 100 + 0.1;
    if (force < 0) return SDL_TRUE;

    setPixelInASurface(wall->surface, x, y, 0, 0, 0, 1);
    wall->texture = SDL_CreateTextureFromSurface(app.renderer, wall->surface);
    if (!wall->texture)
    {
        printf("Error creating texture - %s\n", SDL_GetError());
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    boom(x + 1, y, force, depth, wall);
    boom(x - 1, y, force, depth, wall);
    boom(x, y + 1, force, depth, wall);

    return SDL_TRUE;
}

void wallBoom(Entity* listOfWalls) {
    for (Entity* t = listOfWalls; t != NULL; t = t->next) {
        if (!t->health) {
            boom(t->dx, t->dy, 3, 40, t);
            // SDL_Delay(2000);

            t->health = 1;
        }
    }
}