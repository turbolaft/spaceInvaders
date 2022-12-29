#ifndef DRAW_H
#define DRAW_H

SDL_Surface* loadSurface(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
void blit(SDL_Texture* texture, int x, int y, int w, int h);
void prepareScene();
void presentScene();
SDL_Texture* get_texture(const char* image_filename);
void setPixelInASurface(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, int depth);

#endif