#ifndef HANDLINGPLAYER_H
#define HANDLINGPLAYER_H

Entity* makeBullet();
void drawBullets(Entity** listOfBullets, Entity* listOfEnemies, Entity** addedBullet,
        Entity** listOfEnemyBullets, Entity** addedEnemyBullet, Interval** tearUnderLineIntervals,
        Interval** addedTearUnderLineInterval, Entity* listOfWalls);
void handlePlayer(Entity** firstBullet, Entity** addedBullet);
SDL_bool checkOnDeadPlayer(Entity* bullet);
void* downloadingText(TTF_Font* font, const char* buffer);
void presentingLivesInLow(TTF_Font* font, const int lives);

#endif