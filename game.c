#include "game.h"
#include <SDL.h>
#include <stdbool.h>
#include "MainMenu.h"
#include "MenuWin.h"
#include "MenuLoose.h"

// Fonction pour vérifier l'intersection de deux rectangles
bool checkIntersection(SDL_Rect rect1, SDL_Rect rect2, SDL_Rect *intersection) {
    return SDL_IntersectRect(&rect1, &rect2, intersection);
}
// Fonction pour vérifier si les valeurs random sont présentes dans le tableau
bool isValueInArray(int value, int array[], int size) {
    for (int i = 0; i < size; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}
// Fonction pour définir la couleur d'une brique en fonction de sa vie et de son avantage
void setBrickColor(SDL_Renderer *renderer, int life, int advantage) {
    if (advantage == 0) {
        if (life == 3) {
            SDL_SetRenderDrawColor(renderer, 255, 102, 102, 255);
        } else if (life == 2) {
            SDL_SetRenderDrawColor(renderer, 0, 204, 102, 255);
        } else if (life == 1) {
            SDL_SetRenderDrawColor(renderer, 0, 102, 204, 255);
        }
    } else {
        if (advantage == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        } else if (advantage == 2) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        } else if (advantage == 3) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
    }
}
// Fonction pour changer la direction de la balle lors de la collision avec une brique
void changeDirectionBall(struct ball *b, struct brick br) {
    // Collision par la droite
    if (b->x + b->w >= br.x && b->x < br.x && (b->y + b->h > br.y && b->y < br.y + br.h)) {
        b->speedX = -b->speedX;  // Inversez la direction horizontale
    }
        // Collision par la gauche
    else if (b->x <= br.x + br.w && b->x + b->w > br.x + br.w && (b->y + b->h > br.y && b->y < br.y + br.h)) {
        b->speedX = -b->speedX;  // Inversez la direction horizontale
    }
        // Collision par le bas
    else if (b->y + b->h >= br.y && b->y < br.y && (b->x + b->w > br.x && b->x < br.x + br.w)) {
        b->speedY = -b->speedY;  // Inversez la direction verticale
    }
        // Collision par le haut
    else if (b->y <= br.y + br.h && b->y + b->h > br.y + br.h && (b->x + b->w > br.x && b->x < br.x + br.w)) {
        b->speedY = -b->speedY;  // Inversez la direction verticale
    }
}
// Fonction pour changer la direction de la balle lors de la collision avec le joueur
void changeDirectionBallPlayer(struct ball *b, struct player p, SDL_Rect intersectionRect, int currentPlayerWidth) {
    // Ajuster la direction verticale de la balle en fonction de la collision avec le joueur
    b->speedY = -b->speedY;
    // Ajuster la direction horizontale en fonction de la position de la collision sur le joueur
    int playerCenterX = p.x + currentPlayerWidth / 2;
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
void stopPlayer(struct player *p, int currentPlayerWidth){
    if (p->x + p->speed <= 0) {
        p->x = 0;
    }
    if (p->x + currentPlayerWidth + p->speed > 1280) {
        p->x = 1280 - currentPlayerWidth;
    }
}
// Fonction pour gérer la collision de la balle avec les murs
void collisionBallWall(struct ball *b){
    if (b->x <= 0 || b->x + b->w >= 1280) {
        b->speedX = -b->speedX;
    }
    if (b->y <= 0) {
        b->speedY = -b->speedY;
    }
}
// Fonction pour déplacer la balle
void startBall(struct ball *b){
    b->y += b->speedY;
    b->x += b->speedX;
}
// Fonction pour gérer la sortie de la balle
void exitBall (struct ball *b, int *heart){
    if (b->y + b->h >= 720) {
        (*heart)--;
        b->attachedToPlayer = true;
    }
}
// Fonction pour charger les textures
void loadTextures(SDL_Renderer *renderer, SDL_Texture **texture_player, SDL_Texture **jeu_background, SDL_Texture **bubble, SDL_Texture **texture_heart, SDL_Texture **texture_crab) {
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

    // chargement de l'image pour la vie
    SDL_Surface *surface_heart = IMG_Load("../img/icon_life.png");
    *texture_heart = SDL_CreateTextureFromSurface(renderer, surface_heart);
    SDL_FreeSurface(surface_heart);

    // Chargement de l'image pour la balle
    SDL_Surface *crab = IMG_Load("../img/crab.png");
    *texture_crab = SDL_CreateTextureFromSurface(renderer, crab);
    SDL_FreeSurface(crab);
}
int randomX(int existingValues[], int numExisting) {
    int newValue;
    do {
        newValue = rand() % 8;
    } while (isValueInArray(newValue, existingValues, numExisting));
    return newValue;
}
int randomYOrAdv(){
    return rand() % 3 + 1;
}
// Fonction pour dessiner l'atout de vie
void drawPrimeLife(SDL_Renderer *renderer, struct primeFall *pF ,SDL_Texture *texture_heart) {
    if (pF->active) {
        SDL_Rect redSquareRect = {pF->x, pF->y, pF->w, pF->h};
        SDL_RenderCopy(renderer, texture_heart, NULL, &redSquareRect);
        pF->y +=  pF->speedY;
        if (pF->y + pF->h >= 720) {
            pF->active = false;
        }
    }
}
// Fonction pour dessiner l'atout de balle du joueur
void drawPrimeBall(SDL_Renderer *renderer, struct primeFall *pF2, SDL_Texture *texture_player) {
    if (pF2->active) {
        SDL_Rect redSquareRect = {pF2->x, pF2->y, pF2->w, pF2->h};
        SDL_RenderCopy(renderer, texture_player, NULL, &redSquareRect);
        pF2->y += pF2->speedY;
        if (pF2->y + pF2->h >= 720) {
            pF2->active = false;
        }
    }
}
// Fonction pour dessiner l'atout de tir
void drawPrimeShoot(SDL_Renderer *renderer, struct primeFall *pF3, SDL_Texture *texture_crab) {
    if (pF3->active) {
        SDL_Rect redSquareRect = {pF3->x, pF3->y, pF3->w, pF3->h};
        SDL_RenderCopy(renderer, texture_crab, NULL, &redSquareRect);
        pF3->y += pF3->speedY;
        if (pF3->y + pF3->h >= 720) {
            pF3->active = false;
        }
    }
}
// Fonction pour dessiner la balle tirée
void drawBallShoot(SDL_Renderer *renderer, struct shoot *s, SDL_Texture *texture_crab) {
    if (s->active) {
        SDL_Rect redSquareRect = {s->x, s->y, s->w, s->h};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderCopy(renderer, texture_crab, NULL, &redSquareRect);
        s->y += s->speedY;
    }
}

void gameloop(SDL_Renderer *renderer) {
    SDL_bool quit = false;
    SDL_Event event;

    //Nombre de vies du joueur
    int heart = 3;
    // Configuration des briques
    const int numBricks = 8;
    const int numBricksRow = 3;
    const int brickWidth = 157;
    const int brickHeight = 40;
    //Nombre de briques avec un atout
    int numRandom = 6;
    //taille du joueur
    int currentPlayerWidth = 150;


    int totalBricks = numBricks * numBricksRow;

    //Structure des éléments du jeu
    struct brick bricks[numBricks][numBricksRow];
    struct player p = {360, 700, 10, currentPlayerWidth, 10};
    struct ball b = {300, 300, 20, 20, true,1, 6};
    struct life l = {20, 20, 20, 20};
    struct primeFall pF = {250, 300, 20, 20};
    struct primeFall pF2 = {250, 300, 6, 30};
    struct primeFall pF3 = {250, 300, 20, 20};
    struct shoot s = {250,300,20,20};

    int randomsX[numRandom];
    int randomsY[numRandom];
    //Generation des coordonnées pour les atouts du joueur
    for (int i = 0; i < 6; ++i) {
        randomsX[i] = randomX(randomsX, i);
        randomsY[i] = randomYOrAdv();
    }
    // Variables pour le contrôle du déplacement du joueur
    bool q_press = false;
    bool d_press = false;

    // Gestion du temps pour le mouvement du joueur
    int lastTimer = SDL_GetTicks();
    int gestionFPS = SDL_GetTicks();

    // Chargement des textures du jeu
    SDL_Texture *texture_player;
    SDL_Texture *jeu_background;
    SDL_Texture *bubble;
    SDL_Texture *texture_heart;
    SDL_Texture *texture_crab;

    loadTextures(renderer, &texture_player, &jeu_background, &bubble, &texture_heart, &texture_crab);

    // Initialisation des briques en fonction du niveau
    for (int j = 0; j < numBricksRow; ++j) {
        for (int i = 0; i < numBricks; ++i) {
            bricks[i][j].x = i * (brickWidth + 5); // Espacement des briques en x
            bricks[i][j].y = j * (brickHeight + 10) + 50; // Espacement des briques en y
            bricks[i][j].w = brickWidth;
            bricks[i][j].h = brickHeight;
            bricks[i][j].active = true;
            bricks[i][j].advantage = 0;

            for (int k = 0; k < 2; ++k) {
                    bricks[randomsX[k]][randomsY[k]].advantage = 1;
                    bricks[randomsX[k]][randomsY[k]].life = 1;

            }
            for (int k = 2; k < 4; ++k) {
                    bricks[randomsX[k]][randomsY[k]].advantage = 2;
                    bricks[randomsX[k]][randomsY[k]].life = 1;

            }
            for (int k = 4; k < 6; ++k) {
                    bricks[randomsX[k]][randomsY[k]].advantage = 3;
                    bricks[randomsX[k]][randomsY[k]].life = 1;

            }
            if (currentLevel == 1) {
                bricks[i][j].life = 1;
            } else if (currentLevel == 2) {
                if (j < 2) {
                    bricks[i][j].life = 1;
                }else if (bricks[i][j].advantage == 1 || bricks[i][j].advantage == 2 || bricks[i][j].advantage == 3){
                    bricks[i][j].life = 1;
                }
                else {
                    bricks[i][j].life = 2;
                }
            } else if (currentLevel == 3) {
                if (j == 0) {
                    bricks[i][j].life = 1;
                } else if (j == 1) {
                    bricks[i][j].life = 2;
                } else if (bricks[i][j].advantage == 1 || bricks[i][j].advantage == 2 || bricks[i][j].advantage == 3) {
                    bricks[i][j].life = 1;
                } else {
                    bricks[i][j].life = 3;
                }
            } else if (currentLevel == 4) {
                if (j == 0 || j == 1) {
                    bricks[i][j].life = 2;
                } else if (bricks[i][j].advantage == 1 || bricks[i][j].advantage == 2 || bricks[i][j].advantage == 3) {
                    bricks[i][j].life = 1;
                } else {
                    bricks[i][j].life = 3;
                }
            } else if (currentLevel == 5) {
                if (j == 0 || j == 1) {
                    bricks[i][j].life = 3;
                } else if (bricks[i][j].advantage == 1 || bricks[i][j].advantage == 2 || bricks[i][j].advantage == 3) {
                    bricks[i][j].life = 1;
                } else {
                    bricks[i][j].life = 3;
                }
            }
        }
    }


    while (!quit) {
        while (SDL_PollEvent(&event)) {
            //Gestion des touches
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                menu(renderer);
                return;
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
        // Gestion du temps pour maintenir une fréquence de rafraîchissement constante
        if ( SDL_GetTicks() - gestionFPS > 16 ) {

            stopPlayer(&p, currentPlayerWidth);
            if (b.attachedToPlayer) {
                b.x = p.x + currentPlayerWidth / 2 - b.w / 2;
                b.y = p.y - b.h;
            }
            startBall(&b);
            collisionBallWall(&b);
            exitBall(&b, &heart);

            //Dessin du background
            SDL_RenderCopy(renderer, jeu_background, NULL, NULL);

            // Dessin du joueur
            SDL_Rect playerRect = {p.x, p.y, currentPlayerWidth, p.h};
            SDL_RenderCopy(renderer, texture_player, NULL, &playerRect);

            // Dessin de la balle
            SDL_Rect ballRect = {b.x, b.y, b.w, b.h};
            SDL_RenderCopy(renderer, bubble, NULL, &ballRect);

            //Dessin des vies
            for (int i = 0; i < (heart); ++i) {
                SDL_Rect lifeRect = {l.x + i * (l.w + 10), l.y, l.w, l.h};
                SDL_RenderCopy(renderer, texture_heart, NULL, &lifeRect);
            }

            // Dessin de l'atout carre vie
            SDL_Rect primeLifeRect = {pF.x, pF.y, pF.w, pF.h};
            SDL_RenderCopy(renderer, NULL, NULL, &primeLifeRect);


            // Dessin de l'atout carre aggrandissement joueur
            SDL_Rect primePlayerRect = {pF2.x, pF2.y, pF2.w, pF2.h};
            SDL_RenderCopy(renderer, NULL, NULL, &primePlayerRect);

            //Dessin de l'atout carre de tir
            SDL_Rect primeShootRect = {pF3.x, pF3.y, pF3.w, pF3.h};
            SDL_RenderCopy(renderer, NULL, NULL, &primeShootRect);

            //dessin de l'atout balle tire
            SDL_Rect shootBallRect = {s.x, s.y, s.w, s.h};
            SDL_RenderCopy(renderer, NULL, NULL, &shootBallRect);

            // Vérification de la collision entre le joueur et la balle
            SDL_Rect intersectionRect;
            if (checkIntersection(playerRect, ballRect, &intersectionRect)) {
                changeDirectionBallPlayer(&b, p, intersectionRect, currentPlayerWidth);
                SDL_RenderFillRect(renderer, &intersectionRect);
            }

            // Dessin des briques
            for (int i = 0; i < numBricks; ++i) {
                for (int j = 0; j < numBricksRow; ++j) {
                    if (bricks[i][j].active) {
                        SDL_Rect brickRect = {bricks[i][j].x, bricks[i][j].y, bricks[i][j].w, bricks[i][j].h};
                        setBrickColor(renderer, bricks[i][j].life, bricks[i][j].advantage);
                        SDL_RenderFillRect(renderer, &brickRect);

                        // Verification des collisions
                        if (checkIntersection(brickRect, ballRect, &intersectionRect)) {
                            changeDirectionBall(&b, bricks[i][j]);
                            bricks[i][j].life--;
                            if (bricks[i][j].life == 0) {
                                bricks[i][j].active = false;
                                totalBricks--;

                                if (bricks[i][j].advantage == 1) {
                                    pF.active = true;
                                    pF.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF.y = bricks[i][j].y;
                                    pF.speedY = 3;
                                }
                                if (bricks[i][j].advantage == 2) {
                                    pF2.active = true;
                                    pF2.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF2.y = bricks[i][j].y;
                                    pF2.speedY = 3;
                                }
                                if (bricks[i][j].advantage == 3) {
                                    pF3.active = true;
                                    pF3.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF3.y = bricks[i][j].y;
                                    pF3.speedY = 3;
                                }
                            }

                            SDL_RenderFillRect(renderer, &intersectionRect);
                        }
                        if (checkIntersection(brickRect, shootBallRect, &intersectionRect)) {
                            changeDirectionBall(&b, bricks[i][j]);
                            bricks[i][j].life--;
                            if (s.shoot <5){
                                s.shoot++;
                                s.y = p.y - s.h;
                                s.x = p.x + currentPlayerWidth / 2 - s.w / 2;
                                drawBallShoot(renderer, &s, texture_crab);
                                s.speedY = -7;
                            }else{
                                s.active = false;
                                s.x = - 100;
                                s.y = -100;
                            }
                            if (bricks[i][j].life == 0) {
                                bricks[i][j].active = false;
                                totalBricks--;

                                if (bricks[i][j].advantage == 1) {
                                    pF.active = true;
                                    pF.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF.y = bricks[i][j].y;
                                    pF.speedY = 3;
                                }
                                if (bricks[i][j].advantage == 2) {
                                    pF2.active = true;
                                    pF2.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF2.y = bricks[i][j].y;
                                    pF2.speedY = 3;
                                }
                                if (bricks[i][j].advantage == 3) {
                                    pF3.active = true;
                                    pF3.x = bricks[i][j].x + bricks[i][j].w / 2 - 10;
                                    pF3.y = bricks[i][j].y;
                                    pF3.speedY = 3;
                                }
                            }

                            SDL_RenderFillRect(renderer, &intersectionRect);
                        }
                    }
                }
            }
            //verification des collisions avec des briques qui possede un atout
            drawPrimeLife(renderer, &pF, texture_heart);
            if (checkIntersection(playerRect, primeLifeRect, &intersectionRect)) {
                pF.x = -100;
                pF.y = -100;
                pF.active = false;
                heart++;
            }

            drawPrimeBall(renderer, &pF2, texture_player);
            if (checkIntersection(playerRect, primePlayerRect, &intersectionRect)) {
                pF2.x = -100;
                pF2.y = -100;
                pF2.active = false;
                currentPlayerWidth += 50;
            }
            drawPrimeShoot(renderer, &pF3, texture_crab);
            if (checkIntersection(playerRect, primeShootRect, &intersectionRect)) {
                pF3.x = -100;
                pF3.y = -100;
                pF3.active = false;
                s.shoot = 0;
                s.active = true;
                s.y = p.y - s.h;
                s.x = p.x + currentPlayerWidth / 2 - s.w / 2;

            }
            if (s.active == true){
                drawBallShoot(renderer, &s, texture_crab);
                s.speedY = -7;
            }
            if (s.y <= 0) {
                if (s.shoot <5){
                    s.shoot++;
                    s.y = p.y - s.h;
                    s.x = p.x + currentPlayerWidth / 2 - s.w / 2;
                    drawBallShoot(renderer, &s, texture_crab);
                    s.speedY = -7;
                }else{
                    s.active = false;
                    s.x = - 100;
                    s.y = -100;
                }

            }
            if (s.shoot == 5){
                s.active = false;
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
                if (d_press == true) {
                    p.x += p.speed;
                }
                if (q_press == true) {
                    p.x -= p.speed;
                }

                lastTimer = SDL_GetTicks();
            }
            gestionFPS = SDL_GetTicks();
        }
    }
    SDL_RenderClear(renderer);
}