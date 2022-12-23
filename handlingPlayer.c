#include "structs.h"
#include "defs.h"

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

void drawBullets(Entity** list) {
    Entity* t = *list;

    while (t) {
        Entity* pointerOnT = t;
        t->y -= t->dy;

        if (t->y <= 0) {
            t->health = 0;
        }

        if (t->health) {
            for (int i = 0; i < t->width; i++) {
                SDL_RenderDrawLine(app.renderer, t->x + i, t->y - t->height, t->x + i, t->y);
            }
            t = t->next;
        } else {

            if (*list == t) {
                *list = t->next;
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


    if (app.fire && *firstBullet == NULL && player.reload == 20) {
        *firstBullet = makeBullet();
        *addedBullet = *firstBullet;
        player.reload = 0;
    } else if (app.fire && player.reload == 20) {
        (*addedBullet)->next = makeBullet();
        *addedBullet = (*addedBullet)->next;
        player.reload = 0;
    }
}
