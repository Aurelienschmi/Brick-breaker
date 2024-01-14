
#ifndef BRICKBREAKER_MAINMENU_H
#define BRICKBREAKER_MAINMENU_H

#endif //BRICKBREAKER_MAINMENU_H
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


void displayTitle(SDL_Renderer *renderer, char* path);
void displaySubTitle(SDL_Renderer *renderer, char* path, int r, int g, int b);
void menu(SDL_Renderer* renderer);
