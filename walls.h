#ifndef WALLS_H
#define WALLS_H

Entity* gettingWall(int x, int y);
void spawningWalls(Entity** listOfWalls);
void drawingWalls(Entity* listOfWalls);
void checkOnWallTouch(Entity* listOfWalls, Entity* bullet, int isBoom);
void wallBoom(Entity* listOfWalls);
void boom(int x, int y, float force, int depth, Entity* wall);

#endif