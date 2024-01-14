#include <SDL.h>
#include <stdbool.h>
#include "MainMenu.h"
#include "EasterEgg.h"
#include "MenuLoose.h"
#include "MenuWin.h"

// Fonction pour vérifier l'intersection de deux rectangles
bool checkIntersectionE(SDL_Rect rect1, SDL_Rect rect2, SDL_Rect *intersection) {
    return SDL_IntersectRect(&rect1, &rect2, intersection);
}
// Fonction pour changer la direction de la balle lors de la collision avec une brique
void changeDirectionBallE(struct ballE *b, struct brickE *br) {
// Collision par la droite
    if (b->x + b->w >= br->x && b->x < br->x && (b->y + b->h > br->y && b->y < br->y + br->h)) {
        b->speedX = -b->speedX;
    }
        // Collision par la gauche
    else if (b->x <= br->x + br->w && b->x + b->w > br->x + br->w && (b->y + b->h > br->y && b->y < br->y + br->h)) {
        b->speedX = -b->speedX;
    }
        // Collision par le bas
    else if (b->y + b->h >= br->y && b->y < br->y && (b->x + b->w > br->x && b->x < br->x + br->w)) {
        b->speedY = -b->speedY;
    }
        // Collision par le haut
    else if (b->y <= br->y + br->h && b->y + b->h > br->y + br->h && (b->x + b->w > br->x && b->x < br->x + br->w)) {
        b->speedY = -b->speedY;
    }
}
// Fonction pour changer la direction de la balle lors de la collision avec le joueur
void changeDirectionBallPlayerE(struct ballE *b, struct playerE p, SDL_Rect intersectionRect) {
    // Ajuster la direction verticale de la balle en fonction de la collision avec le joueur
    b->speedY = -b->speedY;

    // Ajuster la direction horizontale en fonction de la position de la collision sur le joueur
    int playerCenterX = p.x + p.w / 2;
    int collisionPointX = intersectionRect.x + intersectionRect.w / 2;

    // Calculer la différence entre le centre du joueur et le point de collision
    int horizontalDifference = collisionPointX - playerCenterX;

    b->speedX = horizontalDifference / 10;

    if (b->speedX > 10) {
        b->speedX = 10;
    } else if (b->speedX < -10) {
        b->speedX = -10;
    }
}
// Fonction pour arrêter le déplacement du joueur sur les cotés
void stopPlayerE(struct playerE *p){
    // Blocage du mouvement du joueur aux bords de l'écran
    if (p->x + p->speed <= 0) {
        p->x = 0; //blocage mur gauche
    }
    if (p->x + p->w + p->speed > 1280) {
        p->x = 1280 - p->w; //blocage mur droit
    }
}
// Fonction pour gérer la collision de la balle avec les murs
void collisionBallWallE(struct ballE *b){
    // Inversion de la direction de la balle sur collision avec les bords horizontaux
    if (b->x <= 0 || b->x + b->w >= 1280) {
        b->speedX = -b->speedX;
    }
    // Inversion de la direction de la balle sur collision avec les bords verticaux
    if (b->y <= 0) {
        b->speedY = -b->speedY;
    }
}
// Fonction pour déplacer la balle
void startBallE(struct ballE *b){
    // Mise à jour de la position de la balle
    b->y += b->speedY;
    b->x += b->speedX;
}
// Fonction pour gérer la sortie de la balle
void exitBallE(struct ballE *b, int *heart){
    // Si la balle atteint le bas
    if (b->y + b->h >= 720) {
        (*heart)--;
        b->attachedToPlayer = true;
    }
}
// Fonction pour charger les textures
void loadTexturesE(SDL_Renderer *renderer, SDL_Texture **texture_player, SDL_Texture **jeu_background, SDL_Texture **bubble, SDL_Texture **texture_brick, SDL_Texture **texture_heart) {
    // Chargement de l'image pour le joueur
    SDL_Surface *surface_player = IMG_Load("../img/joueur.png");
    *texture_player = SDL_CreateTextureFromSurface(renderer, surface_player);
    SDL_FreeSurface(surface_player);

    // Chargement de l'image pour le background
    SDL_Surface *image = IMG_Load("../img/jeu_background.jpg");
    *jeu_background = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    // Chargement de l'image pour la balle
    SDL_Surface *ball = IMG_Load("../img/bubble.png");
    *bubble = SDL_CreateTextureFromSurface(renderer, ball);
    SDL_FreeSurface(ball);

    // Chargement de l'image pour la brique
    SDL_Surface *shark = IMG_Load("../img/brick.png");
    *texture_brick= SDL_CreateTextureFromSurface(renderer, shark);
    SDL_FreeSurface(shark);

    // chargement de l'image pour la vie
    SDL_Surface *surface_heart = IMG_Load("../img/icon_life.png");
    *texture_heart = SDL_CreateTextureFromSurface(renderer, surface_heart);
    SDL_FreeSurface(surface_heart);
}

void startEasterEgg(SDL_Renderer *renderer) {
    bool quit = false;
    SDL_Event event;

    //Nombre de vies du joueur
    int heart = 3;
    // Configuration des briques
    const int numBricks = 6;
    const int numBricksRow = 2;
    const int brickWidth = 210;
    const int brickHeight = 150;
    int currentPlayerWidth = 150;

    int totalBricks = numBricks * numBricksRow;
    //Structure des éléments du jeu
    struct brickE bricks[numBricks][numBricksRow];
    struct playerE p = {360, 700, 10, currentPlayerWidth, 10};
    struct ballE b = {300, 300, 20, 20, true, 1, 1};
    struct lifeE l = {20, 20, 20, 20};


    // Variables pour le contrôle du déplacement du joueur
    bool q_press = false;
    bool d_press = false;

    // Gestion du temps pour le mouvement du joueur
    int lastTimer = SDL_GetTicks();

    // Chargement des textures du jeu
    SDL_Texture *texture_player;
    SDL_Texture *jeu_background;
    SDL_Texture *bubble;
    SDL_Texture *texture_brick;
    SDL_Texture *texture_heart;

    loadTexturesE(renderer, &texture_player, &jeu_background, &bubble, &texture_brick, &texture_heart);


    // Initialisation des briques
    for (int j = 0; j < numBricksRow; ++j) {
        for (int i = 0; i < numBricks; ++i) {
            bricks[i][j].x = i * (brickWidth + 5); // Espacement des briques en x
            bricks[i][j].y = j * (brickHeight + 10) + 50; // Espacement des briques en y
            bricks[i][j].w = brickWidth;
            bricks[i][j].h = brickHeight;
            bricks[i][j].active = true;
            bricks[i][j].life = 2;
        }
    }


    while (!quit) {
        while (SDL_PollEvent(&event)) {
            //Gestion des touches
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                menu(renderer);
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        q_press = true;
                        if (b.attachedToPlayer) {
                            b.x -= p.speed;
                        }
                        break;
                    case SDLK_d:
                        d_press = true;
                        if (b.attachedToPlayer) {
                            b.x += p.speed;
                        }
                        break;
                    case SDLK_SPACE:
                        b.attachedToPlayer = false;
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        q_press = false;
                        break;
                    case SDLK_d:
                        d_press = false;
                        break;
                }
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                return;
            }
        }
        stopPlayerE(&p);
        if (b.attachedToPlayer) {
            b.x = p.x + p.w / 2 - b.w / 2;
            b.y = p.y - b.h;
        }
        startBallE(&b);

        collisionBallWallE(&b);
        exitBallE(&b, &heart);
        if (totalBricks == 0) {
            quit = true;
            menu(renderer);
        }
        //Initialisation du background
        SDL_RenderCopy(renderer, jeu_background, NULL, NULL);

        // Dessin du joueur
        SDL_Rect playerRect = {p.x, p.y, currentPlayerWidth, p.h};
        SDL_RenderCopy(renderer, texture_player, NULL, &playerRect);

        // Dessin de la balle
        SDL_Rect ballRect = {b.x, b.y, b.w, b.h};
        SDL_RenderCopy(renderer, bubble, NULL, &ballRect);

        //Dessin de la vie
        for (int i = 0; i < heart; ++i) {
            SDL_Rect lifeRect = {l.x + i * (l.w + 10), l.y, l.w, l.h};
            SDL_RenderCopy(renderer, texture_heart, NULL, &lifeRect);
        }

        // Vérification de la collision entre le joueur et la balle
        SDL_Rect intersectionRect;
        if (checkIntersectionE(playerRect, ballRect, &intersectionRect)) {
            changeDirectionBallPlayerE(&b, p, intersectionRect);
            SDL_RenderFillRect(renderer, &intersectionRect);
        }

        // Dessin des briques
        for (int i = 0; i < numBricks; ++i) {
            for (int j = 0; j < numBricksRow; ++j) {
                if (bricks[i][j].active) {
                    SDL_Rect brickRect = {bricks[i][j].x, bricks[i][j].y, bricks[i][j].w, bricks[i][j].h};
                    SDL_RenderCopy(renderer, texture_brick, NULL, &brickRect);
                    // Vérification de collision
                    if (checkIntersectionE(brickRect, ballRect, &intersectionRect)) {
                        changeDirectionBallE(&b, &bricks[i][j]);
                        bricks[i][j].life--;
                        if (bricks[i][j].life == 0) {
                            bricks[i][j].active = false;
                            totalBricks--;
                        }

                        SDL_RenderFillRect(renderer, &intersectionRect);
                    }
                }
            }
        }

        //Verification des conditions d'arret de jeu
        if (totalBricks == 0) {
            quit = true;
            menuWin(renderer);
        }
        if (heart == 0) {
            quit = true;
            menuLoose(renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        // Gestion du mouvement du joueur en fonction du temps
        if (SDL_GetTicks() - lastTimer > 16) {
            lastTimer = SDL_GetTicks();
            if (d_press == true) {
                p.x += p.speed;
            }
            if (q_press == true) {
                p.x -= p.speed;
            }
        }
        SDL_Delay(3);
    }
}
