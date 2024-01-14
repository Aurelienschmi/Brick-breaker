//
// Created by schmi on 19/12/2023.
//

#ifndef MAIN_C_SETTINGSMENU_H
#define MAIN_C_SETTINGSMENU_H

#endif //MAIN_C_SETTINGSMENU_H

#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int currentLevel = 1;

void displaySettings(SDL_Renderer *renderer, char* path);
void displayTitleLevel(SDL_Renderer *renderer, char* path);
void level(SDL_Renderer *renderer, char* path, int r, int g, int b, int levelNumber, SDL_Rect dest);
void settingsMenu(SDL_Renderer* renderer) ;