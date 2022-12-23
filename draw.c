#include "structs.h"

void prepareScene() {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
    SDL_RenderClear(app.renderer);
}

void presentScene() {
    SDL_RenderPresent(app.renderer);
}

SDL_Surface* loadSurface(const char* path) {
    SDL_Surface* surface = IMG_Load(path);

    if (!surface) {
        printf("Error creating a surface\n");
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    return surface;
}

SDL_Texture* loadTexture(SDL_Surface* surface) {
    SDL_Texture* tex = SDL_CreateTextureFromSurface(app.renderer, surface);
    SDL_FreeSurface(surface);

    if (!tex) {
        printf("Error creating texture - %s\n", SDL_GetError());
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        SDL_Quit();
        exit(1);
    }

    return tex;
}

void blit(SDL_Texture* texture, int x, int y) {
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    dest.w = 100;
    dest.h = 50;
    player.width = dest.w;
    player.height = dest.h;

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

// SDL_Texture * get_texture(const char* image_filename) {
//     SDL_Texture * result = NULL;

//     SDL_Surface * pSurface = IMG_Load(image_filename);

//     if (pSurface == NULL) {
//         printf("Error image load: %s\n", IMG_GetError());
//     }
//     else {
//         const int limit = 1024;
//         int width = pSurface->w;
//         int height = pSurface->h;

//         if ((width > limit) ||
//             (height > limit)) {
//             SDL_Rect sourceDimensions;
//             sourceDimensions.x = 0;
//             sourceDimensions.y = 0;
//             sourceDimensions.w = width;
//             sourceDimensions.h = height;

//             float scale = (float)limit / (float)width;
//             float scaleH = (float)limit / (float)height;

//             if (scaleH < scale) {
//                 scale = scaleH;
//             }

//             SDL_Rect targetDimensions;
//             targetDimensions.x = 0;
//             targetDimensions.y = 0;
//             // targetDimensions.w = (int)(width * scale);
//             // targetDimensions.h = (int)(height * scale);
//             targetDimensions.w = 100;
//             targetDimensions.h = 100;

//             // create a 32 bits per pixel surface to Blit the image to first, before BlitScaled
//             // https://stackoverflow.com/questions/33850453/sdl2-blit-scaled-from-a-palettized-8bpp-surface-gives-error-blit-combination/33944312
//             SDL_Surface *p32BPPSurface = SDL_CreateRGBSurface(
//                 pSurface->flags,
//                 sourceDimensions.w,
//                 sourceDimensions.h,
//                 8,
//                 pSurface->format->Rmask,
//                 pSurface->format->Gmask,
//                 pSurface->format->Bmask,
//                 pSurface->format->Amask);

//             if (SDL_BlitSurface(pSurface, NULL, p32BPPSurface, NULL) < 0) {
//                 printf("Error did not blit surface: %s\n", SDL_GetError());
//             }
//             else {
//                 // create another 32 bits per pixel surface are the desired scale
//                 SDL_Surface *pScaleSurface = SDL_CreateRGBSurface(
//                     p32BPPSurface->flags,
//                     targetDimensions.w,
//                     targetDimensions.h,
//                     p32BPPSurface->format->BitsPerPixel,
//                     p32BPPSurface->format->Rmask,
//                     p32BPPSurface->format->Gmask,
//                     p32BPPSurface->format->Bmask,
//                     p32BPPSurface->format->Amask);

//                 // 32 bit per pixel surfaces (loaded from the original file) won't scale down with BlitScaled, suggestion to first fill the surface
//                 // 8 and 24 bit depth pngs did not require this
//                 // https://stackoverflow.com/questions/20587999/sdl-blitscaled-doesnt-work
//                 SDL_FillRect(pScaleSurface, &targetDimensions, SDL_MapRGBA(pScaleSurface->format, 255, 0, 0, 255));

//                 if (SDL_BlitScaled(p32BPPSurface, NULL, pScaleSurface, NULL) < 0) {
//                     printf("Error did not scale surface: %s\n", SDL_GetError());

//                     SDL_FreeSurface(pScaleSurface);
//                     pScaleSurface = NULL;
//                 }
//                 else {
//                     SDL_FreeSurface(pSurface);

//                     pSurface = pScaleSurface;
//                     width = pSurface->w;
//                     height = pSurface->h;
//                 }
//             }

//             SDL_FreeSurface(p32BPPSurface);
//             p32BPPSurface = NULL;
//         }

//         SDL_Texture * pTexture = SDL_CreateTextureFromSurface(app.renderer, pSurface);

//         if (pTexture == NULL) {
//             printf("Error image load: %s\n", SDL_GetError());
//         }
//         else {
//             SDL_SetTextureBlendMode(
//                 pTexture,
//                 SDL_BLENDMODE_BLEND);

//             result = pTexture;
//         }

//         SDL_FreeSurface(pSurface);
//         pSurface = NULL;
//     }

//     return result;
// }
