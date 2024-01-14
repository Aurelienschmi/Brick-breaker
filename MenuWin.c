#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "MainMenu.h"
// Fonction pour initialiser une zone de texte
void displayTitleWin(SDL_Renderer *renderer, char* path){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {0,102,204};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "You Win", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {70, 250, 400,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
// Fonction pour initialiser une seconde zone de texte
void displaySubTitleWin(SDL_Renderer *renderer, char* path, int r, int g, int b){
    TTF_Font* font = TTF_OpenFont (path, 200);
    SDL_Color color = {r,g,b};
    SDL_Surface* surface = TTF_RenderText_Blended(font, "Press SPACE to return to the menu", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dest = {100, 350, 600,100};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}


void menuWin(SDL_Renderer* renderer){
    //gestion de la couleur
    int r = 255;
    int g = 255;
    int b = 255;

    //Chargement du background
    SDL_Surface* image = IMG_Load("../img/menu_background.jpg");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);


    SDL_bool quit = false;
    SDL_Event event;
    bool mainMenu = false;



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
                mainMenu = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Vérifier si le clic est dans la zone du sous-titre
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect subTitleRect = {100, 350, 600,100};
                if (mouseX >= subTitleRect.x && mouseX < subTitleRect.x + subTitleRect.w &&
                    mouseY >= subTitleRect.y && mouseY < subTitleRect.y + subTitleRect.h) {
                    mainMenu = true;
                    break;
                }
            }
            if (event.type == SDL_MOUSEMOTION){
                //verification pour connaitre la position du curseur et changer la couleur si il est sur le sous-titre
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                SDL_Rect subTitleRect = {100, 350, 600,100};
                if (mouseX >= subTitleRect.x && mouseX < subTitleRect.x + subTitleRect.w && mouseY >= subTitleRect.y && mouseY < subTitleRect.y + subTitleRect.h) {
                    r = 30;
                    g = 144;
                    b = 200;
                }else {
                    r = 255;
                    g = 255;
                    b = 255;
                }
            }
            if (mainMenu){
                menu(renderer);
                quit = true;
            }

            // Afficher le sous-titre avec la couleur mise à jour
            displayTitleWin(renderer, "../fonts/findet-nemo/Findet Nemo.ttf");
            displaySubTitleWin(renderer, "../fonts/findet-nemo/Findet Nemo.ttf", r, g, b);
            SDL_RenderPresent(renderer);
        }
    }
}