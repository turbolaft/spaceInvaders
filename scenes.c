#include "common.h"
#include "structs.h"
#include "defs.h"
#include "draw.h"
#include "handlingPlayer.h"
#include "input.h"

void showTransitionalScene(int playerLives, TTF_Font* SansBold) {
    char buffer[50];
    snprintf(buffer, 50, "%d Lives left", playerLives);
    
    SDL_Rect Message_rect;
    Message_rect.x = SCREEN_WIDTH;
    Message_rect.h = 100;
    Message_rect.w = 300;
    Message_rect.y = SCREEN_HEIGHT / 2 - Message_rect.h / 2;

    SDL_Texture* looseMessage = (SDL_Texture*) downloadingText(SansBold, buffer);

    int closeWindowInAfterLooseScene = 0;

    while (Message_rect.x + Message_rect.w >= 0 && !closeWindowInAfterLooseScene) {
        prepareScene();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    closeWindowInAfterLooseScene = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			            closeWindowInAfterLooseScene = 1;
		            }
                    break;
                default:
                    break;
            }
        }

        Message_rect.x -= 8;
        SDL_RenderCopy(app.renderer, looseMessage, NULL, &Message_rect);
        presentScene();
        SDL_Delay(1000/60);
    }

    SDL_DestroyTexture(looseMessage);
}

void showGameOverScene(TTF_Font* SansBold, TTF_Font* smallSans) {
    char bufferWhenGameIsOver[50];
    SDL_Texture* Message = (SDL_Texture*) downloadingText(SansBold, "The Game is over");
    
    SDL_Rect Message_rect;
    Message_rect.h = 250;
    Message_rect.w = 500;
    Message_rect.x = SCREEN_WIDTH / 2 - Message_rect.w / 2;
    Message_rect.y = SCREEN_HEIGHT / 2 - Message_rect.h / 2;

    SDL_Rect warnMessage_rect;
    warnMessage_rect.h = 100;
    warnMessage_rect.w = 300;
    warnMessage_rect.x = 10;
    warnMessage_rect.y = 10;

    int closeWindowInAfterLooseScene = 0;
    int clockCounter = 5;

    while (clockCounter && !closeWindowInAfterLooseScene) {
        prepareScene();
        doInput(&closeWindowInAfterLooseScene);
        snprintf(bufferWhenGameIsOver, 50, "The window will authomaticaly close in %d", clockCounter);
        SDL_Texture* warnMessage = (SDL_Texture*) downloadingText(smallSans, bufferWhenGameIsOver);

        clockCounter--;
        SDL_RenderCopy(app.renderer, Message, NULL, &Message_rect);
        SDL_RenderCopy(app.renderer, warnMessage, NULL, &warnMessage_rect);
        SDL_DestroyTexture(warnMessage);
        presentScene();
        if (!closeWindowInAfterLooseScene) SDL_Delay(1000);
    }

    SDL_DestroyTexture(Message);
}