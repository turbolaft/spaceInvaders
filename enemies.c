#include "common.h"
#include "structs.h"
#include "defs.h"
#include "draw.h"

Entity* makeEnemyBullet(Entity* enemy) {
    Entity* enemyBullet = NULL;

    enemyBullet = (Entity*) malloc(sizeof(Entity));
    memset(enemyBullet, 0, sizeof(Entity));
    enemyBullet->width = 9;
    enemyBullet->height = 30;
    enemyBullet->y = enemy->y + enemy->height;
    enemyBullet->x = (enemy->x + enemy->width / 2) - enemyBullet->width / 2;
    enemyBullet->health = 1;
    enemyBullet->dy = 10;
    enemyBullet->next = NULL;
    enemyBullet->texture = enemyBulletTexture;

    return enemyBullet;
}

Entity* makeExtraEnemy() {
    Entity* enemyBullet = NULL;

    enemyBullet = (Entity*) malloc(sizeof(Entity));
    memset(enemyBullet, 0, sizeof(Entity));
    enemyBullet->width = 100;
    enemyBullet->height = 35;
    enemyBullet->y = 60;
    enemyBullet->x = SCREEN_WIDTH;
    enemyBullet->health = 1;
    enemyBullet->dx = 10;
    enemyBullet->texture = extraEnemyTexture;

    return enemyBullet;
}

void spawnEnemies(Entity** first, Entity** added) {
    for (int i = 0; i < RAWS_OF_ENEMIES; i++) {
        for (int j = 0; j < COLUMNS_OF_ENEMIES; j++) {
            Entity* enemy = NULL;
            enemy = (Entity*) malloc(sizeof(Entity));
            memset(enemy, 0, sizeof(Entity));
            enemy->height = 35;
            enemy->width = 50;
            enemy->x = 20 + (enemy->width * j + ENEMIES_GAP_X * j);
            enemy->y = TOP_OFFSET_ENEMIES + (enemy->height * i + ENEMIES_GAP_Y * i);
            enemy->health = 1;
            enemy->direction = 1;
            enemy->tick = 0;
            enemy->reload = ENEMIES_TICK_DELAY;
            enemy->dx = ENEMIES_SPEED;
            enemy->next = NULL;

            if (i == 0) {
                enemy->texture = enemyFirstRowBTexture;
            } else if (i == 1) {
                enemy->texture = enemySecondRowBTexture;
            } else if (i > 1) {
                enemy->texture = enemyOtherRowsBTexture;
            }

            if (i == 0 && j == 0) {
                *first = enemy;
                *added = *first;
            } else {
                (*added)->next = enemy;
                *added = (*added)->next;
            }
        }
    }

    *added = NULL;
}

void representingEnemies(Entity** list) {
    Entity* t = *list;
    Entity* prevT = NULL;

    while(t) {
        blit(t->texture, t->x, t->y, t->width, t->height);

        if (!t->health) {
            if (t->reload == 100 - ENEMIES_TICK_DELAY) {
                if (prevT) {
                    prevT->next = t->next;
                } else {
                    *list = t->next;        
                }
                Entity* pointerOnT = t;
                t = t->next;
                free(pointerOnT); 
            } else {
                t->reload--;
                prevT = t;
                t = t->next;
            }
        } else {
            prevT = t;
            t = t->next;
        }
    }
}

void handlingEnemy(Entity** listOfEnemies, Entity** listOfEnemyBullets, Entity** addedEnemyBullet) {
    Entity* t = *listOfEnemies;

    if (t) {
        int currentEnemyTick;
        int isMovementAllowed = 0;
        int leftmostPosition = 0;
        int rightmostPosition = 0;
        int numberOfEnemiesAlive = 0;
        int isShootingAllowed = 0;

        if (t->reload == ENEMIES_TICK_DELAY) {
            currentEnemyTick = !t->tick;
            isMovementAllowed = 1;
            t->reload = 0;
        } else {
            currentEnemyTick = t->tick;
        }

        if ((t->reload % (ENEMIES_TICK_DELAY - 10) == 0 && t->reload / (ENEMIES_TICK_DELAY - 10) == 1)
                || (t->reload % 10 == 0 && t->reload / 10 == 1)) {
            isShootingAllowed = 1;
        }

        while (t) {
            if (!leftmostPosition) {
                leftmostPosition = t->x;
            } 

            if (!rightmostPosition) {
                rightmostPosition = t->x + t->width;
            }

            rightmostPosition = rightmostPosition < (t->x + t->width) ? t->x + t->width : rightmostPosition;
            leftmostPosition = leftmostPosition > t->x ? t->x : leftmostPosition;

            if (t->health) numberOfEnemiesAlive++;
            t = t->next;
        }

        Entity* t = *listOfEnemies;
        int randomEnemyToShoot = rand() % (numberOfEnemiesAlive + 1);
        int countOnLoop = 0;

        if ((rightmostPosition + 20) >= SCREEN_WIDTH) t->direction = 0;
        else if ((leftmostPosition - 20) <= 0) t->direction = 1;

        int currentDirection = t->direction;

        while(t) {
            t->direction = currentDirection;
            t->tick = currentEnemyTick;

            if (t->direction && isMovementAllowed) {
                t->x += t->dx;
            } else if (isMovementAllowed) {
                t->x -= t->dx;
            }

            if (t->tick && t->health) {
                if (t->y == TOP_OFFSET_ENEMIES) {
                    t->texture = enemyFirstRowATexture;
                } else if (t->y == (TOP_OFFSET_ENEMIES + t->height + ENEMIES_GAP_Y)) {
                    t->texture = enemySecondRowATexture;
                } else if (t->y >= (TOP_OFFSET_ENEMIES + t->height + ENEMIES_GAP_Y)) {
                    t->texture = enemyOtherRowsATexture;
                }
            } else if (!t->tick && t->health) {
                if (t->y == TOP_OFFSET_ENEMIES) {
                    t->texture = enemyFirstRowBTexture;
                } else if (t->y == (TOP_OFFSET_ENEMIES + t->height + ENEMIES_GAP_Y)) {
                    t->texture = enemySecondRowBTexture;
                } else if (t->y >= (TOP_OFFSET_ENEMIES + t->height + ENEMIES_GAP_Y)) {
                    t->texture = enemyOtherRowsBTexture;
                }
            }
            
            if (!t->health && t->reload <= ENEMIES_TICK_DELAY) {
                t->texture = deadEnemyTexture;
                t->reload = 100;
            }

            if (countOnLoop == randomEnemyToShoot && isShootingAllowed && t->health) {
                if (*listOfEnemyBullets == NULL) {
                    *listOfEnemyBullets = makeEnemyBullet(t);
                    *addedEnemyBullet = *listOfEnemyBullets;
                } else {
                    (*addedEnemyBullet)->next = makeEnemyBullet(t);
                    *addedEnemyBullet = (*addedEnemyBullet)->next;
                }
            }

            if (t->reload < ENEMIES_TICK_DELAY && t->health) t->reload++;
            if (t->health) countOnLoop++;
            t = t->next;
        }

        representingEnemies(listOfEnemies);
    }
}

SDL_bool checkOnDeadEnemy(Entity* bullet, Entity* listOfEnemies) {
    Entity* t = listOfEnemies;

    while (t) {
        const SDL_Rect enemy = {t->x, t->y, t->width, t->height};

        for (int i = 0; i < bullet->width; i++) {
            for (int j = 0; j < bullet->height; j++) {
                const SDL_Point bulletPoint = {bullet->x + i, bullet->y + j};

                if (SDL_PointInRect(&bulletPoint, &enemy) == SDL_TRUE && t->health) {
                    bullet->health = 0;
                    player.score += 10;
                    t->health = 0;
                    return SDL_TRUE;
                }
            }
        }

        t = t->next;
    }
    return SDL_FALSE;
}

void spawnExtraEnemy(Entity* extraEnemy) {
    if (extraEnemy->tick == EXTRA_ENEMY_APPEARANCES) {
        extraEnemy->x -= extraEnemy->dx;
        blit(extraEnemy->texture, extraEnemy->x, extraEnemy->y, extraEnemy->width, extraEnemy->height);

        if ((extraEnemy->x + extraEnemy->width) <= 0 || !extraEnemy->health) {
            extraEnemy->x = SCREEN_WIDTH;
            extraEnemy->health = 1;
            extraEnemy->tick = 0;
        }
    } else {
        extraEnemy->tick++;
    }

}

void checkingOnDeadExtraEnemy(Entity* bullet, Entity* extraEnemy) {
    const SDL_Rect enemy = {extraEnemy->x, extraEnemy->y, extraEnemy->width, extraEnemy->height};

    for (int i = 0; i < bullet->width; i++) {
        for (int j = 0; j < bullet->height; j++) {
            const SDL_Point bulletPoint = {bullet->x + i, bullet->y + j};

            if (SDL_PointInRect(&bulletPoint, &enemy) == SDL_TRUE) {
                bullet->health = 0;
                player.score += 40;
                extraEnemy->health = 0;
                return;
            }
        }
    }
}