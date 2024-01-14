//
// Created by schmi on 04/12/2023.
//

#ifndef BRICKBREAKER_GAME_H
#define BRICKBREAKER_GAME_H

#endif //BRICKBREAKER_GAME_H
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern int currentLevel;

struct player{
    int x;
    int y;
    int h;
    int w;

    int speed;
};

struct ball{
    int x;
    int y;
    int h;
    int w;
    bool attachedToPlayer;
    int speedX ;
    int speedY;
};
struct brick {
    int x;
    int y;
    int h;
    int w;
    bool active;
    int life;
    int advantage;
};

struct life {
    int x;
    int y;
    int h;
    int w;
};
struct primeFall{
    int x;
    int y;
    int h;
    int w;
    int speedY;
    bool active;
};

struct shoot{
    int x;
    int y;
    int h;
    int w;
    int speedY;
    bool active;
    int shoot;
};



void gameloop(SDL_Renderer *renderer);
