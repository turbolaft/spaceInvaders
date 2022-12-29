#include "common.h"
#include "structs.h"
#include "draw.h"
#include "defs.h"
#include "enemies.h"
#include "underLine.h"
#include "walls.h"

Entity* makeBullet() {
    Entity* bullet;
    bullet = (Entity*) malloc(sizeof(Entity));
    bullet->height = 20;
    bullet->dy = BULLET_SPEED;
    bullet->width = 2;
    bullet->y = player.y;
    bullet->x = player.x + player.width / 2;
    bullet->health = 1;
    bullet->next = NULL;

    return bullet;
}

SDL_bool checkOnDeadPlayer(Entity* bullet) {
    const SDL_Rect playerRect = {player.x, player.y, player.width, player.height};
    
    for (int i = 0; i < bullet->width; i++) {
        for (int j = 0; j < bullet->height; j++) {
            const SDL_Point bulletPoint = {bullet->x + i, bullet->y + j};

            if (SDL_PointInRect(&bulletPoint, &playerRect) == SDL_TRUE) {
                bullet->health = 0;
                player.health = 0;
                return SDL_TRUE;
            }
        }
    }

    return SDL_FALSE;
}

void drawBullets(Entity** listOfBullets, Entity* listOfEnemies, Entity** addedBullet,
        Entity** listOfEnemyBullets, Entity** addedEnemyBullet, Interval** tearUnderLineIntervals,
        Interval** addedTearUnderLineInterval, Entity* listOfWalls) {
    Entity* t = *listOfBullets;
    Entity* prevT = NULL;

    while (t) {
        Entity* pointerOnT = t;
        t->y -= t->dy;

        if (t->y <= 0) {
            t->health = 0;
        }

        checkOnDeadEnemy(t, listOfEnemies);
        checkOnWallTouch(listOfWalls, t, 0);

        if (t->health) {
            for (int i = 0; i < t->width; i++) {
                SDL_RenderDrawLine(app.renderer, t->x + i, t->y - t->height, t->x + i, t->y);
            }

            prevT = t;
            *addedBullet = t;
            t = t->next;
        } else {

            if (*listOfBullets == t) {
                *listOfBullets = t->next;
                *addedBullet = t->next;
            } else if (prevT) {
                prevT->next = t->next;
                *addedBullet = prevT;
            }

            t = t->next;
            free(pointerOnT);           
        }
    }

    t = *listOfEnemyBullets;
    prevT = NULL;

    while (t) {
        Entity* pointerOnT = t;
        t->y += t->dy;

        if (t->y >= SCREEN_HEIGHT) {
            t->health = 0;
        }

        checkingOnUnderlineTear(tearUnderLineIntervals, t, addedTearUnderLineInterval);
        checkOnDeadPlayer(t);
        checkOnWallTouch(listOfWalls, t, 1);

        if (t->health) {
            blit(t->texture, t->x, t->y, t->width, t->height);

            prevT = t;
            *addedEnemyBullet = t;
            t = t->next;
        } else {

            if (*listOfEnemyBullets == t) {
                *listOfEnemyBullets = t->next;
                *addedEnemyBullet = t->next;
            } else if (prevT) {
                prevT->next = t->next;
                *addedEnemyBullet = prevT;
            }

            t = t->next;
            free(pointerOnT);           
        }
    }
}

void handlePlayer(Entity** firstBullet, Entity** addedBullet) {
    float x_vel = 0;

    if (app.left && !app.right) {
        x_vel = -PLAYER_SPEED;
    }

    if (app.right && !app.left) {
        x_vel = PLAYER_SPEED;
    }

    player.x += (int) x_vel / 60;

    if (player.x <= 0) player.x = 0;
    if (player.x >= (SCREEN_WIDTH - player.width)) player.x = SCREEN_WIDTH - player.width;


    if (app.fire && (*firstBullet == NULL || *addedBullet == NULL) && player.reload == PLAYER_RELOAD) {
        *firstBullet = makeBullet();
        *addedBullet = *firstBullet;
        player.reload = 0;
    } else if (app.fire && player.reload == PLAYER_RELOAD) {
        (*addedBullet)->next = makeBullet();
        *addedBullet = (*addedBullet)->next;
        player.reload = 0;
    }
}

void* downloadingText(TTF_Font* font, const char* buffer) {
    const SDL_Color white = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, buffer, white);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);

    return Message;
}

void presentingLivesInLow(TTF_Font* font, const int lives) {
    char buffer[3];
    SDL_Rect coordinates = {5, 660, 30, player.height};
    snprintf(buffer, 3, "%d", lives);
    SDL_Texture* textTexture = (SDL_Texture*) downloadingText(font, buffer);
    blit(textTexture, coordinates.x, coordinates.y, coordinates.w, coordinates.h);
    SDL_DestroyTexture(textTexture);

    for (int i = 1; i <= lives; i++) {
        blit(player.texture, coordinates.x + 5 * i + player.width * i, coordinates.y, player.width, player.height);
    }
}
