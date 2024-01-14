#include "MainMenu.h"
#include "game.h"
#include "InformationMenu.h"
#include "MenuEasterEgg.h"
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
// Fonction pour initialiser une zone de texte
void displayTitle(SDL_Renderer *renderer, char* path){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {0,102,204};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Brick Breaker", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {70, 250, 400,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
// Fonction pour initialiser une seconde zone de texte
void displaySubTitle(SDL_Renderer *renderer, char* path, int r, int g, int b){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {r,g,b};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Press SPACE to play", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {100, 350, 400,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}


void menu(SDL_Renderer* renderer){
    int r = 255;
    int g = 255;
    int b = 255;

    //Chargement de l'icône paramètre s'il n'est pas survolé
    SDL_Surface *surface_settings = IMG_Load("../img/settings1.png");
    SDL_Texture *texture_settings = SDL_CreateTextureFromSurface(renderer, surface_settings);
    SDL_FreeSurface(surface_settings);
    SDL_Rect settingsIconRect = {1200, 25, 50, 50};

    //Chargement de l'icône paramètre si il est survolé
    SDL_Surface *surface_settings2 = IMG_Load("../img/settings2.png");
    SDL_Texture *texture_settings2 = SDL_CreateTextureFromSurface(renderer, surface_settings2);
    SDL_FreeSurface(surface_settings2);
    SDL_Rect settingsIconRect2 = {1200, 25, 50, 50};

    //Chargement du background
    SDL_Surface* image = IMG_Load("../img/menu_background.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //Chargement de l'icon pour l'Easter egg
    SDL_Surface* surface_easter_egg = IMG_Load("../img/icon sharks.png");
    SDL_Texture* texture_easter_egg = SDL_CreateTextureFromSurface(renderer, surface_easter_egg);
    SDL_FreeSurface(surface_easter_egg);
    SDL_Rect easter_egg = {1000, 680, 50, 50};

    //Chargement de l'icône information s'il n'est pas survolé
    SDL_Surface* surface_information = IMG_Load("../img/information_icon.png");
    SDL_Texture* texture_information = SDL_CreateTextureFromSurface(renderer, surface_information);
    SDL_FreeSurface(surface_information);
    SDL_Rect information = {100, 25, 50, 50};

    //Chargement de l'icône information si il est survolé
    SDL_Surface* surface_information2 = IMG_Load("../img/information_icon2.png");
    SDL_Texture* texture_information2 = SDL_CreateTextureFromSurface(renderer, surface_information2);
    SDL_FreeSurface(surface_information2);
    SDL_Rect information2 = {100, 25, 50, 50};

    SDL_bool quit = false;
    SDL_Event event;
    bool game = false;
    bool settings = false;
    bool easterEgg = false;
    bool informationMenu = false;


    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                quit = true;
                break;
            }
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
                game = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Vérifier si le clic est dans la zone du sous-titre
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect subTitleRect = {100, 350, 400, 100};
                if (mouseX >= subTitleRect.x && mouseX < subTitleRect.x + subTitleRect.w &&
                    mouseY >= subTitleRect.y && mouseY < subTitleRect.y + subTitleRect.h) {
                    game = true;
                    break;
                }
                SDL_Rect settingsRect = {1200, 25, 50, 50};
                if (mouseX >= settingsRect.x && mouseX < settingsRect.x + settingsRect.w &&
                    mouseY >= settingsRect.y && mouseY < settingsRect.y + settingsRect.h) {
                    settings = true;
                    break;
                }
                SDL_Rect easterEggRect = {1000, 680, 50, 50};
                if (mouseX >= easterEggRect.x && mouseX < easterEggRect.x + easterEggRect.w &&
                    mouseY >= easterEggRect.y && mouseY < easterEggRect.y + easterEggRect.h) {
                    easterEgg = true;
                    break;
                }
                SDL_Rect information1 = {100, 25, 50, 50};
                if (mouseX >= information1.x && mouseX < information1.x + information1.w &&
                    mouseY >= information1.y && mouseY < information1.y + information1.h) {
                    informationMenu = true;
                    break;
                }
            }
            if (event.type == SDL_MOUSEMOTION){
                //verification pour connaitre la position du curseur et changer la couleur pour les sous-titres ou d'image pour les parametre et les options
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect subTitleRect = {100, 350, 400, 100};
                if (mouseX >= subTitleRect.x && mouseX < subTitleRect.x + subTitleRect.w && mouseY >= subTitleRect.y && mouseY < subTitleRect.y + subTitleRect.h) {
                    r = 30;
                    g = 144;
                    b = 200;
                }else {
                    r = 255;
                    g = 255;
                    b = 255;
                }
                SDL_Rect settingsRect = {1200, 25, 50, 50};
                if (mouseX >= settingsRect.x && mouseX < settingsRect.x + settingsRect.w &&
                    mouseY >= settingsRect.y && mouseY < settingsRect.y + settingsRect.h) {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_easter_egg, NULL, &easter_egg);
                    SDL_RenderCopy(renderer, texture_settings2, NULL, &settingsIconRect2);
                    SDL_RenderCopy(renderer, texture_information,NULL,&information);

                } else {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_easter_egg, NULL, &easter_egg);
                    SDL_RenderCopy(renderer, texture_settings, NULL, &settingsIconRect);
                    SDL_RenderCopy(renderer, texture_information2,NULL,&information2);

                }
                SDL_Rect information1 = {100, 25, 50, 50};
                if (mouseX >= information1.x && mouseX < information1.x + information1.w &&
                    mouseY >= information1.y && mouseY < information1.y + information1.h) {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_easter_egg, NULL, &easter_egg);
                    SDL_RenderCopy(renderer, texture_settings, NULL, &settingsIconRect);
                    SDL_RenderCopy(renderer, texture_information,NULL,&information);

                } else {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderCopy(renderer, texture_easter_egg, NULL, &easter_egg);
                    SDL_RenderCopy(renderer, texture_settings, NULL, &settingsIconRect);
                    SDL_RenderCopy(renderer, texture_information2,NULL,&information2);
                }
            }

            if (game){
                gameloop(renderer);
                quit = true;
            }
            if (settings){
                menu(renderer);
                quit = true;
            }
            if (easterEgg){
                menuEasterEgg(renderer);
                quit = true;
            }
            if (informationMenu){
                informationMenu1(renderer);
                quit = true;
            }
            // Afficher le sous-titre avec la couleur mise à jour

            displayTitle(renderer, "../fonts/findet-nemo/Findet Nemo.ttf");
            displaySubTitle(renderer, "../fonts/findet-nemo/Findet Nemo.ttf", r, g, b);
            SDL_RenderPresent(renderer);
        }
    }
}