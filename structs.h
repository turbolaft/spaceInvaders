#ifndef STRUCTS_H
#define STRUCTS_H

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
    int fire;
    int reload;
    int score;
    // if direction is 1 means right, if 0 - left
    int direction;
    // tick for enemies to set their image type(a - 1 or b - 0)
    int tick;
    SDL_Surface* surface;
    struct Entity* next;
    SDL_Texture* texture;
} Entity;

typedef struct Interval{
    int x1;
    int x2;
    struct Interval* next;
} Interval;

App app;
Entity player;

#endif