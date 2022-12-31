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
    SDL_Texture* looseBackground = loadTexture(loadSurface("resources/background1.jpg"));

    int closeWindowInAfterLooseScene = 0;

    while (Message_rect.x + Message_rect.w >= 0 && !closeWindowInAfterLooseScene) {
        prepareScene();
        blit(looseBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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
    FILE* bestScores = fopen("resources/bestScores.txt", "r+");

    if (!bestScores) {
        printf("Error opening a file\n");
        exit(1);
    }

    int lines = 0;

    while(!feof(bestScores)) {
        char ch;
        ch = fgetc(bestScores);
        if(ch == '\n'){
            lines++;
        }
    }

    rewind(bestScores);

    char result[20];
    int* resultsInNum = (int*) malloc((lines + 2) * sizeof(int));
    memset(resultsInNum, 0, lines * sizeof(int));
    int* singleElement = resultsInNum;

    for (int i = 0; i <= lines; i++) {
        fgets(result, 20, bestScores);
        if (!strcmp(result, "s")) {
            lines--;
        }
        *(singleElement + i) = atoi(result);
        if (i == lines) singleElement = singleElement + i + 1;
    }

    int temp = 0;
    *singleElement = player.score;

    for (int i = 0; i <= (lines + 1); ++i){
        for (int j = i + 1; j <= (lines + 1); ++j){
            if (*(resultsInNum + i) < *(resultsInNum + j)) {
                temp = *(resultsInNum + i);
                *(resultsInNum + i) = *(resultsInNum + j);
                *(resultsInNum + j) = temp;
            }
        }
    }

    for (int i = 0; i <= (lines + 1); i++) {
        printf("%d\n", *(resultsInNum + i));
    }

    fclose(bestScores);

    FILE* bestScoresWrite = fopen("resources/bestScores.txt", "w");

    for (int i = 0; i < 10 && i <= (lines + 1); i++) {
        char modificatedScore[20];
        if (i == 9 || i == lines + 1) {
            sprintf(modificatedScore, "%d", *(resultsInNum + i));
        } else {
            sprintf(modificatedScore, "%d\n", *(resultsInNum + i));
        }
        fputs(modificatedScore, bestScoresWrite);
    }

    fclose(bestScoresWrite);

    SDL_Texture* gameOverBackground = loadTexture(loadSurface("resources/background.jpg"));
    SDL_Texture* gameOverText = (SDL_Texture*) downloadingText(SansBold, "Game over");
    SDL_Texture* bestScoresText = (SDL_Texture*) downloadingText(SansBold, "Best scores");

    prepareScene();
    SDL_Rect coordinates;
    coordinates.w = 200;
    coordinates.h = 50;
    coordinates.x = SCREEN_WIDTH / 2 - coordinates.w / 2;
    coordinates.y = SCREEN_HEIGHT / 2 - (coordinates.h / 2) * lines - 20;
    blit(gameOverBackground, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    blit(gameOverText, coordinates.x, 10, coordinates.w, coordinates.h);
    blit(bestScoresText, coordinates.x, coordinates.y - coordinates.h - 10, coordinates.w, coordinates.h);

    for (int i = 0; i < 10 && i <= (lines + 1); i++) {
        char buffer[30];
        sprintf(buffer, "%d.  ............%d", i + 1, *(resultsInNum + i)); 
        SDL_Texture* text = (SDL_Texture*) downloadingText(SansBold, buffer);
        blit(text, coordinates.x, coordinates.y + (coordinates.h * i + 10 * i), coordinates.w, coordinates.h);
        SDL_DestroyTexture(text);
    }

    presentScene();

    SDL_Delay(5000);

    SDL_DestroyTexture(gameOverText);
    SDL_DestroyTexture(bestScoresText);
    SDL_DestroyTexture(gameOverBackground);
    free(resultsInNum);
}