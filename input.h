#ifndef INPUT_H
#define INPUT_H

void doInput();
void doKeyDown(SDL_KeyboardEvent* event);
void doKeyUp(SDL_KeyboardEvent* event);
void doInput(int* requestClose);

#endif