#include "common.h"
#include "structs.h"
#include "defs.h"

SDL_bool checkingOnUnderlineTear(Interval** motherInterval, Entity* bullet, Interval** addedInterval) {
    const SDL_Rect underLine = {0, player.y + 55, SCREEN_WIDTH, player.y + 57};

    const SDL_Point bulletPoint = {bullet->x + bullet->width / 2, bullet->y + bullet->height};

    if (SDL_PointInRect(&bulletPoint, &underLine) == SDL_TRUE) {

        if (*motherInterval == NULL) {
            *motherInterval = (Interval*) malloc(sizeof(Interval));
            (*motherInterval)->x1 = bullet->x;
            (*motherInterval)->x2 = bullet->x + bullet->width;
            (*motherInterval)->next = NULL;
            *addedInterval = *motherInterval;
            // printf("%p\n", motherInterval);
        } else {
            (*addedInterval)->next = (Interval*) malloc(sizeof(Interval));
            (*addedInterval)->next->x1 = bullet->x;
            (*addedInterval)->next->x2 = bullet->x + bullet->width;
            *addedInterval = (*addedInterval)->next;
            (*addedInterval)->next = NULL;
            // printf("%p\n", motherInterval);
        }
        
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

void drawingUnderLine(Interval* listOfHurtIntervals) {
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(app.renderer, 0, player.y + 55 + i, SCREEN_WIDTH, player.y + 55 + i);
    }

    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);

    for (Interval* t = listOfHurtIntervals; t != NULL; t = t->next) {
        for (int i = 0; i < 3; i++) {
            SDL_RenderDrawLine(app.renderer, t->x1, player.y + 55 + i, t->x2, player.y + 55 + i);
        }
    }
}