#ifndef ENEMIES_H
#define ENEMIES_H

void spawnEnemies(Entity** first, Entity** added);
void representingEnemies(Entity** list);
void handlingEnemy(Entity** listOfEnemies, Entity** listOfEnemyBullets, Entity** addedEnemyBullet);
SDL_bool checkOnDeadEnemy(Entity* bullet, Entity* listOfEnemies);

#endif