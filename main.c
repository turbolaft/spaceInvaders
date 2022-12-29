#include "common.h"
#include "structs.h"
#include "draw.h"
#include "input.h"
#include "init.h"
#include "defs.h"
#include "handlingPlayer.h"
#include "enemies.h"
#include "underLine.h"
#include "walls.h"

int main() {
    
    int playerLives = 3;
    memset(&app, 0, sizeof(App));

    nextTry:
    memset(&player, 0, sizeof(Entity));

    if (playerLives == 3) {
        initSDL();
    } else {
        player.score -= (60 / playerLives);
    }

    TTF_Font* smallSans = TTF_OpenFont("resources/sans-Bold.ttf", 20);
    TTF_Font* SansBold = TTF_OpenFont("resources/sans-Bold.ttf", 50);

    SDL_Color white = {255, 255, 255};

    player.texture = loadTexture(loadSurface("resources/player.png"));

    Entity* firstBullet = NULL;
    Entity* addedBullet = NULL;
    Entity* firstEnemy = NULL;
    Entity* addedEnemy = NULL;
    Entity* firstEnemyBullet = NULL;
    Entity* addedEnemyBullet = NULL;
    Entity* walls = NULL;
    Interval* underLineTearIntervals = NULL;
    Interval* addedUnderLineTearInterval = underLineTearIntervals;
    
    player.width = 100;
    player.height = 50;
    player.health = 1;
    char scoreBuffer[20];
    SDL_Rect scorePosition = {20, 10, 180, 60};
    spawnEnemies(&firstEnemy, &addedEnemy);
    spawningWalls(&walls);

    player.reload = PLAYER_RELOAD;

    player.x = SCREEN_WIDTH / 2 - player.width / 2;
    player.y = 600;
    int requestClose = 0;

    while(!requestClose) {

        prepareScene();

        doInput(&requestClose);

        handlePlayer(&firstBullet, &addedBullet);

        handlingEnemy(&firstEnemy, &firstEnemyBullet, &addedEnemyBullet);

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        
        drawBullets(&firstBullet, firstEnemy, &addedBullet, &firstEnemyBullet, &addedEnemyBullet,
         &underLineTearIntervals, &addedUnderLineTearInterval, walls);
        if (player.reload != PLAYER_RELOAD) {
            player.reload++;
        }

        drawingUnderLine(underLineTearIntervals);

        blit(player.texture, player.x, player.y, player.width, player.height);

        snprintf(scoreBuffer, 20, "Score: %d", player.score);
        SDL_Texture* scoreTexture = (SDL_Texture*) downloadingText(smallSans, scoreBuffer);

        blit(scoreTexture, scorePosition.x, scorePosition.y, scorePosition.w, scorePosition.h);
        SDL_DestroyTexture(scoreTexture);
        presentingLivesInLow(smallSans, playerLives);

        drawingWalls(walls);

        wallBoom(walls);

        presentScene();

        if (!player.health) {
            requestClose = 1;
            prepareScene();
            presentScene();
        }

        SDL_Delay(1000/60);
    }

    while(walls) {
        Entity* freee = walls;
        SDL_FreeSurface(walls->surface); 
        walls = walls->next;
        // printf("surface - %p\n", walls->surface);
        free(freee);
    }

    while(underLineTearIntervals) {
        Interval* freee = underLineTearIntervals;
        underLineTearIntervals = underLineTearIntervals->next;
        free(freee);
    }

    while(firstEnemyBullet) {
        Entity* freee = firstEnemyBullet;
        firstEnemyBullet = firstEnemyBullet->next;
        free(freee);
    }

    while(firstEnemy) {
        Entity* freee = firstEnemy;
        firstEnemy = firstEnemy->next;
        free(freee);
    }

    while(firstBullet) {
        Entity* freee = firstBullet;
        firstBullet = firstBullet->next;
        free(freee);
    }

    if (!player.health && --playerLives != 0) {

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

            doInput(&closeWindowInAfterLooseScene);
            Message_rect.x -= 8;
            SDL_RenderCopy(app.renderer, looseMessage, NULL, &Message_rect);
            presentScene();
            SDL_Delay(1000/60);
        }

        SDL_DestroyTexture(looseMessage);
        SDL_DestroyTexture(player.texture);
        goto nextTry;
    }

    if (!player.health && !playerLives) {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(SansBold, "The Game is over", white);

        char bufferWhenGameIsOver[50];
        SDL_Texture* Message = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);
        SDL_FreeSurface(surfaceMessage);
        
        SDL_Rect Message_rect;
        Message_rect.h = 250;
        Message_rect.w = 500;
        Message_rect.x = SCREEN_WIDTH / 2 - Message_rect.w / 2;
        Message_rect.y = SCREEN_HEIGHT / 2 - Message_rect.h / 2;

        SDL_Rect warnMessage_rect;
        warnMessage_rect.h = 100;
        warnMessage_rect.w = 400;
        warnMessage_rect.x = 10;
        warnMessage_rect.y = 10;


        int closeWindowInAfterLooseScene = 0;
        int clockCounter = 5;

        while (clockCounter && !closeWindowInAfterLooseScene) {
            prepareScene();
            doInput(&closeWindowInAfterLooseScene);
            snprintf(bufferWhenGameIsOver, 50, "The window will authomaticaly close in %d", clockCounter);
            SDL_Surface* warnSurfaceMessage = TTF_RenderText_Solid(smallSans, bufferWhenGameIsOver, white);

            SDL_Texture* warnMessage = SDL_CreateTextureFromSurface(app.renderer, warnSurfaceMessage);
            SDL_FreeSurface(warnSurfaceMessage);

            clockCounter--;
            SDL_RenderCopy(app.renderer, Message, NULL, &Message_rect);
            SDL_RenderCopy(app.renderer, warnMessage, NULL, &warnMessage_rect);
            SDL_DestroyTexture(warnMessage);
            presentScene();
            SDL_Delay(1000);
        }

        SDL_DestroyTexture(Message);
        SDL_DestroyTexture(player.texture);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    TTF_CloseFont(smallSans);
    TTF_CloseFont(SansBold);
    TTF_Quit();
    SDL_Quit();

    return 0;
}