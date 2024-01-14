//
// Created by schmi on 19/12/2023.
//

#include "SettingsMenu.h"
#include "MainMenu.h"
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

void displaySettings(SDL_Renderer *renderer, char* path){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {0,102,204};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Settings", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {70, 50, 400,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void displayTitleLevel(SDL_Renderer *renderer, char* path){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {255,255,255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Choose the level:", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {70, 200, 400,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
void level(SDL_Renderer *renderer, char* path, int r, int g, int b, int levelNumber, SDL_Rect dest){
    TTF_Font* font = TTF_OpenFont (path, 50);
    SDL_Color color = {r,g,b};
    // Utiliser snprintf pour concaténer le numéro du niveau à la chaîne de texte
    char text[3];  // Assurez-vous que la taille de votre numéro de niveau est correcte
    snprintf(text, sizeof(text), "%d", levelNumber);
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void settingsMenu(SDL_Renderer* renderer) {
    int rSelected = 30;
    int gSelected = 144;
    int bSelected = 200;

    SDL_Surface *image = IMG_Load("../img/menu_background.jpg");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Surface *surface_back = IMG_Load("../img/cross.png");
    SDL_Texture *texture_back = SDL_CreateTextureFromSurface(renderer, surface_back);
    SDL_FreeSurface(surface_back);
    SDL_Rect backIcon = {1200, 25, 50, 50};

    SDL_Surface *surface_back2 = IMG_Load("../img/cross2.png");
    SDL_Texture *texture_back2 = SDL_CreateTextureFromSurface(renderer, surface_back2);
    SDL_FreeSurface(surface_back2);
    SDL_Rect backIcon2 = {1200, 25, 50, 50};

    SDL_bool quit = false;
    SDL_Event event;
    bool mainMenu = false;

    SDL_Rect levelRects[] = {
            {120, 320, 30, 45},
            {170, 320, 30, 45},
            {220, 320, 30, 45},
            {270, 320, 30, 45},
            {320, 320, 30, 45}
    };

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                menu(renderer);
                break;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect settingsRect = {1200, 25, 50, 50};
                if (mouseX >= settingsRect.x && mouseX < settingsRect.x + settingsRect.w &&
                    mouseY >= settingsRect.y && mouseY < settingsRect.y + settingsRect.h) {
                    mainMenu = true;
                    break;
                }

                for (int i = 0; i < 5; i++){
                    if (mouseX >= levelRects[i].x && mouseX < levelRects[i].x + levelRects[i].w &&
                        mouseY >= levelRects[i].y && mouseY < levelRects[i].y + levelRects[i].h) {
                        currentLevel = i + 1;
                        break;
                    }
                }
            }
            if (event.type == SDL_MOUSEMOTION){
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect backRect = {1200, 25, 50, 50};
                if (mouseX >= backRect.x && mouseX < backRect.x + backRect.w &&
                    mouseY >= backRect.y && mouseY < backRect.y + backRect.h) {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_back2, NULL, &backIcon2);
                } else {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_back, NULL, &backIcon);
                }
            }
            if (mainMenu){
            menu(renderer);
            quit = true;
            }
            for (int i = 0; i < 5; i++){
                int levelR, levelG, levelB;
                if (i + 1 == currentLevel) {
                    levelR = rSelected;
                    levelG = gSelected;
                    levelB = bSelected;
                } else {
                    levelR = 255;
                    levelG = 255;
                    levelB = 255;
                }
                level(renderer, "../fonts/Roboto/Roboto-Bold.ttf",levelR,levelG,levelB,i+1,levelRects[i]);
            }


            displaySettings(renderer, "../fonts/findet-nemo/Findet Nemo.ttf");
            displayTitleLevel(renderer, "../fonts/findet-nemo/Findet Nemo.ttf");

            SDL_RenderPresent(renderer);

        }
    }
}