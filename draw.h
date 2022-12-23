#ifndef DRAW_H
#define DRAW_H

SDL_Surface* loadSurface(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
void blit(SDL_Texture* texture, int x, int y);
void prepareScene();
void presentScene();
SDL_Texture* get_texture(const char* image_filename);

#endif