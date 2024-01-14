//
// Created by schmi on 19/12/2023.
//

#include "MainMenu.h"
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>


void informationMenu1(SDL_Renderer* renderer) {

    //Chargement du background
    SDL_Surface *image = IMG_Load("../img/information_menu_page.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //Chargement de l'image pour quitter si elle n'est pas survolée
    SDL_Surface *surface_back = IMG_Load("../img/cross.png");
    SDL_Texture *texture_back = SDL_CreateTextureFromSurface(renderer, surface_back);
    SDL_FreeSurface(surface_back);
    SDL_Rect backIcon = {1200, 25, 50, 50};

    //Chargement de l'image pour quitter si elle est survolée
    SDL_Surface *surface_back2 = IMG_Load("../img/cross2.png");
    SDL_Texture *texture_back2 = SDL_CreateTextureFromSurface(renderer, surface_back2);
    SDL_FreeSurface(surface_back2);
    SDL_Rect backIcon2 = {1200, 25, 50, 50};

    SDL_bool quit = false;
    SDL_Event event;
    bool mainMenu = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                menu(renderer);
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                //Action de retour vers le menu principale si clic dans la zone
                SDL_Rect settingsRect = {1200, 25, 50, 50};
                if (mouseX >= settingsRect.x && mouseX < settingsRect.x + settingsRect.w &&
                    mouseY >= settingsRect.y && mouseY < settingsRect.y + settingsRect.h) {
                    mainMenu = true;
                    break;
                }
            }
            if (event.type == SDL_MOUSEMOTION){
                //verification pour connaitre la position du curseur et changer la couleur si il est sur le sous-titre
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
            SDL_RenderPresent(renderer);

        }
    }
}
