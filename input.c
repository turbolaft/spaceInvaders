#include "structs.h"

void doKeyDown(SDL_KeyboardEvent* event) {
    if (event->repeat == 0) {

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
			app.left = 1;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
			app.right = 1;
		}

        if (event->keysym.scancode == SDL_SCANCODE_LCTRL) {
            app.fire = 1;
        }

    }
}

void doKeyUp(SDL_KeyboardEvent* event) {
    if (event->repeat == 0) {

        if (event->keysym.scancode == SDL_SCANCODE_LEFT) {
			app.left = 0;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT) {
			app.right = 0;
		}

        if (event->keysym.scancode == SDL_SCANCODE_LCTRL) {
            app.fire = 0;
        }

    }
}

void doInput(int* requestClose) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				*requestClose = 1;
				break;
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
			default:
				break;
		}
	}
}