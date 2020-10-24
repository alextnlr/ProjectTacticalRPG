#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"

using namespace std;

int main(int argc, char** argv)
{
    SDL_Window* fenetre;
    SDL_Event evenements;
    int terminer = 0;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur init SDL: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //Creation de fenetre
    int lig = 0;
    int col = 0;
    taille_fichier("terrain.txt", &lig, &col);
    int width = col*64;
    int height = lig*64;
    fenetre = SDL_CreateWindow("Jeu",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_RESIZABLE);
    if(fenetre == NULL) {
        printf("Erreur creation fenetre: %s",SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //Mise en place du renderer
    SDL_Renderer* ecran;
    ecran = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    //Chargement de l'image
    SDL_Texture* fond = charger_image("fond.bmp",ecran);

    //Charger Image avec transparence
    Uint8 r = 0, g = 255, b = 255;
    SDL_Texture* obj = charger_image_transparente("sprites.bmp",ecran, r, g, b);

    int objetW;
    int objetH;
    SDL_QueryTexture(obj, NULL, NULL, &objetW, &objetH);


    SDL_Rect SrcR[6];
    for(unsigned i = 0 ; i < 6 ; i++) {
        SrcR[i].x = i > 2 ? (i-3)*objetW/3 : i*objetW/3;
        SrcR[i].y = i > 2 ? objetH/2 : 0;
        SrcR[i].w = objetW/3;
        SrcR[i].h = objetH/2;
    }

    SDL_Rect DestR;
    DestR.x = 0;
    DestR.y = 0;
    DestR.w = 64;
    DestR.h = 84;

    SDL_Texture* bloc = charger_image("pavage.bmp",ecran);

    int blocW, blocH;
    SDL_QueryTexture(bloc, NULL, NULL, &blocW, &blocH);

    SDL_Rect coupBloc[16*10];
    for(unsigned i = 0 ; i < 160 ; i++) {
        coupBloc[i].x = i%16*blocW/16;
        coupBloc[i].y = i/16*blocH/10;
        coupBloc[i].w = blocW/16;
        coupBloc[i].h = blocH/10;
    }

    SDL_Rect posBloc;
    posBloc.x = 0;
    posBloc.y = 0;
    posBloc.w = 64;
    posBloc.h = 64;

    //Tableau Sprites

    char** tab = lire_fichier("terrain.txt");

    int tabInt[lig][col];
    for(unsigned i = 0 ; i < lig ; i++) {
        for(unsigned j = 0 ; j < col ; j++) {
            char stTemp[2];
            sprintf(stTemp,"%c",tab[i][j]);
            tabInt[i][j] = atoi(stTemp);
        }
    }

    SDL_Rect Hitbox;
    Hitbox = hitbox_update(DestR);

    int xmouse;
    int ymouse;

    int TICK_INTERVAL = 120;

    int frameNum = SDL_GetTicks() / TICK_INTERVAL;
    int physical_frame = 1;
    int mouvement = 0;
    int direction = 0;

    int avance = 5;

    int anim = 0;

    //Boucle principale
    while(!terminer) {
        SDL_RenderClear(ecran);
        SDL_RenderCopy(ecran,fond,NULL,NULL);

        if(SDL_GetTicks() / TICK_INTERVAL > frameNum) {
          frameNum = SDL_GetTicks() / TICK_INTERVAL;
          physical_frame = 1;
        } else {
          physical_frame = 0;
        }

        for(unsigned i = 0 ; i < lig ; i++) {
            posBloc.y = i*64;
            for(unsigned j = 0 ; j < col ; j++) {
                posBloc.x = j*64;
                SDL_RenderCopy(ecran, bloc, &coupBloc[tabInt[i][j]], &posBloc);
            }
        }

        if(physical_frame && mouvement) {
            anim ++;
            if(anim > 2) { anim = 0; }
        }

        if(mouvement) {
            SDL_RenderCopy(ecran, obj, &SrcR[anim+3*direction], &DestR);
        } else {
            SDL_RenderCopy(ecran, obj, &SrcR[1+3*direction], &DestR);
        }

        mouvement = 0;

        SDL_PollEvent(&evenements);
        switch(evenements.type)
        {
            case SDL_QUIT:
                terminer = 1; break;
            case SDL_KEYDOWN:
                switch(evenements.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        terminer = 1; break;
                    case SDLK_UP:
                        DestR.y -= avance;
                        Hitbox = hitbox_update(DestR);
                        if(Hitbox.y < 0) {DestR.y += avance;}
                        if(tabInt[Hitbox.y/64][Hitbox.x/64] != 0 || tabInt[Hitbox.y/64][(Hitbox.x+Hitbox.w)/64] != 0) {DestR.y += avance;}
                        mouvement = 1;
                        break;
                    case SDLK_DOWN:
                        DestR.y += avance;
                        Hitbox = hitbox_update(DestR);
                        if(Hitbox.y+Hitbox.h > height) {DestR.y -= avance;}
                        if(tabInt[(Hitbox.y+Hitbox.h)/64][(Hitbox.x)/64] != 0 || tabInt[(Hitbox.y+Hitbox.h)/64][(Hitbox.x+Hitbox.w)/64] != 0)
                            {DestR.y -= avance;}
                        mouvement = 1;
                        break;
                    case SDLK_RIGHT:
                        DestR.x += avance;
                        Hitbox = hitbox_update(DestR);
                        if(Hitbox.x+Hitbox.w > width) {DestR.x -= avance;}
                        if(tabInt[Hitbox.y/64][(Hitbox.x+Hitbox.w)/64] != 0 || tabInt[(Hitbox.y+Hitbox.h)/64][(Hitbox.x+Hitbox.w)/64] != 0)
                            {DestR.x -= avance;}
                        mouvement = 1;
                        direction = 0;
                        break;
                    case SDLK_LEFT:
                        DestR.x -= avance;
                        Hitbox = hitbox_update(DestR);
                        if(Hitbox.x < 0) {DestR.x += avance;}
                        if(tabInt[Hitbox.y/64][Hitbox.x/64] != 0 || tabInt[(Hitbox.y+Hitbox.h)/64][Hitbox.x/64] != 0)
                            {DestR.x += avance;}
                        mouvement = 1;
                        direction = 1;
                        break;
                    case SDLK_s:
                        ecrire_fichier("newMap.txt",tab,lig,col);
                        break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    switch(evenements.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            SDL_GetMouseState(&xmouse,&ymouse);
                            if(tabInt[ymouse/64][xmouse/64] == 0) {
                                tabInt[ymouse/64][xmouse/64] = 4;
                                tab[ymouse/64][xmouse/64] = '4';
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            SDL_GetMouseState(&xmouse,&ymouse);
                            if(tabInt[ymouse/64][xmouse/64] == 4) {
                                tabInt[ymouse/64][xmouse/64] = 0;
                                tab[ymouse/64][xmouse/64] = '0';
                            }
                            break;
                    }
                }
        }

        SDL_RenderPresent(ecran);
        SDL_Delay(1000./60.);
    }

    desallouer_tab_2D(tab, lig);

    //Quitter SDL
    SDL_DestroyTexture(fond);
    SDL_DestroyTexture(obj);
    SDL_DestroyRenderer(ecran);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
