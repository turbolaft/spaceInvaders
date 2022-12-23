#include "structs.h"
#include "draw.h"
#include "input.h"
#include "init.h"
#include "defs.h"
#include "handlingPlayer.h"

int main() {
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity));
    memset(&bullet, 0, sizeof(Entity));

    initSDL();

    player.texture = loadTexture(loadSurface("resources/player.png"));
    blit(player.texture, 0, 0);

    Entity* firstBullet = NULL;
    Entity* addedBullet = NULL;
    player.reload = 20;

    player.x = SCREEN_WIDTH / 2 - player.width / 2;
    player.y = 600;
    int requestClose = 0;

    while(!requestClose) {
        prepareScene();

        doInput(&requestClose);

        handlePlayer(&firstBullet, &addedBullet); 

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);

        
        drawBullets(&firstBullet);
        if (player.reload != 20) {
            player.reload++;
        }

        for (int i = 0; i < 3; i++) {
            SDL_RenderDrawLine(app.renderer, 0, player.y + 55 + i, SCREEN_WIDTH, player.y + 55 + i);
        }

        blit(player.texture, player.x, player.y);

        presentScene();

        SDL_Delay(1000/60);
    }

    while(firstBullet) {
        Entity* freee = firstBullet;
        firstBullet = firstBullet->next;
        free(freee);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;
}