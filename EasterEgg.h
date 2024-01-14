//
// Created by schmi on 04/12/2023.
//

#ifndef BRICKBREAKER_USERINPUT_H
#define BRICKBREAKER_USERINPUT_H

#endif //BRICKBREAKER_USERINPUT_H

#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

struct playerE{
    int x;
    int y;
    int h;
    int w;

    int speed;
};

struct ballE{
    int x;
    int y;
    int h;
    int w;
    bool attachedToPlayer;
    int speedX ;
    int speedY;
};
struct brickE {
    int x;
    int y;
    int h;
    int w;
    bool active;
    int life;
};
struct lifeE {
    int x;
    int y;
    int h;
    int w;
};


void startEasterEgg(SDL_Renderer *renderer);
