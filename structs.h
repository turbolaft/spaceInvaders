#ifndef STRUCTS_H
#define STRUCTS_H

#include "common.h"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    int left;
    int right;
    int fire;
} App;

typedef struct Entity {
    int x;
    int y;
    int width;
    int height;
    int dx;
    int dy;
    int health;
    int reload;
    struct Entity* next;
    SDL_Texture* texture;
} Entity;

App app;
Entity player;
Entity bullet;
#endif