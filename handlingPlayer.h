#ifndef HANDLINGPLAYER_H
#define HANDLINGPLAYER_H

Entity* makeBullet();
void drawBullets(Entity** list);
void handlePlayer(Entity** firstBullet, Entity** addedBullet);

#endif