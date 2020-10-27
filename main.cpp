#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "personnage.h"

using namespace std;

int main(int argc, char** argv)
{
    SDL_Window* fenetre;
    SDL_Event evenements;
    int terminer = 0;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur init SDL: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    TTF_Init();

    //Creation de fenetre
    int lig = 0;
    int col = 0;
    taille_fichier("terrain.txt", &lig, &col);
    int width = col*64;
    int height = lig*64;
    fenetre = SDL_CreateWindow("MageBattle",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_RESIZABLE);
    if(fenetre == NULL) {
        cout << "Erreur creation fenetre: " << SDL_GetError() << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //Mise en place du renderer
    SDL_Renderer* ecran;
    ecran = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    //Chargement de l'image
    SDL_Texture* fond = charger_image("fond.bmp",ecran);

    //Charger Image avec transparence
    //Uint8 r = 0, g = 255, b = 255;
    //SDL_Texture* obj = charger_image_transparente("sprites.bmp",ecran, r, g, b);

    //Texte ttf
    TTF_Font* fontText = TTF_OpenFont("prstartk.ttf",20);
    if(!fontText) {
        cout << "Erreur du chargement de la font" << endl;
    }


    //Option Bloc
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
    for(int i = 0 ; i < lig ; i++) {
        for(int j = 0 ; j < col ; j++) {
            tabInt[i][j] = tab[i][j] - '0';
        }
    }

    /*SDL_Rect Hitbox;
    Hitbox = hitbox_update(DestR);*/

    //Test Objets

    Uint8 r = 128, g = 160, b = 128;
    SDL_Texture* mageTexture = charger_image_transparente("MageM.bmp",ecran, r, g, b);

    vector<Personnage> ennemis;
    ennemis.push_back(Personnage(30, 7, "Thierry", 8, 4, mageTexture));
    ennemis.push_back(Personnage(40, 11, "Caro", 6, 8, mageTexture));

    vector<Personnage> allies;
    allies.push_back(Personnage(60, 5, "Michel", mageTexture));
    allies.push_back(Personnage(40, 10, "Jean", 2, 2, mageTexture));


    int xmouse;
    int ymouse;

    bool found = false;

    int select = 0;
    allies[select].selectPerso();

    int TICK_INTERVAL = 60;

    int frameNum = SDL_GetTicks() / TICK_INTERVAL;
    int physical_frame = 1;
    bool mouvement = 0;
    int direction = 0;
    int cptTime = 0;
    bool wait = false;

    //int anim = 0;

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

        for(int i = 0 ; i < lig ; i++) {
            posBloc.y = i*64;
            for(int j = 0 ; j < col ; j++) {
                posBloc.x = j*64;
                SDL_RenderCopy(ecran, bloc, &coupBloc[tabInt[i][j]], &posBloc);
            }
        }

        /*if(physical_frame && mouvement) {
            anim ++;
            if(anim > 2) { anim = 0; }
        }

        if(mouvement) {
            SDL_RenderCopy(ecran, obj, &SrcR[anim+3*direction], &DestR);
        } else {
            SDL_RenderCopy(ecran, obj, &SrcR[1+3*direction], &DestR);
        }*/

        //SDL_RenderCopy(ecran, texte, NULL, &texte_pos);

        for(int i = 0 ; i < ennemis.size() ; i++)
        {
            ennemis[i].afficherPersoBarre(ecran, physical_frame);
        }

        for(int i = 0 ; i < allies.size() ; i++)
        {
            allies[i].afficherPersoBarre(ecran, physical_frame);
        }

        for(int i = 0 ; i < ennemis.size() ; i++)
        {
            ennemis[i].afficherDegats(ecran, fontText);
        }

        //mouvement = 0;

        if(select != -1)
        {
            if(allies[select].getAgro())
            {
                allies[select].afficherSelect(ecran);
            }
            if(allies[select].getAgro())
            {
                allies[select].setState(5);
            } else if(mouvement)
            {
                allies[select].setState(direction+1);
            } else
            {
                allies[select].setState(0);
            }
        }

        if(physical_frame && frameNum%2) {
                mouvement = false;
                wait = false;
        }

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
                        if(physical_frame && !wait && select != -1)
                        {
                            if(allies[select].getAgro())
                            {
                                allies[select].select(0);
                            } else {
                                allies[select].walk(0);
                                allies[select].setState(0);
                                direction = 0;
                                mouvement = true;
                                wait = true;
                            }
                        }
                        break;
                    case SDLK_DOWN:
                        if(physical_frame && !wait && select != -1)
                        {
                            if(allies[select].getAgro())
                            {
                                allies[select].select(1);
                            } else {
                                allies[select].walk(1);
                                allies[select].setState(1);
                                direction = 1;
                                mouvement = true;
                                wait = true;
                            }
                        }
                        break;
                    case SDLK_RIGHT:
                        if(physical_frame && !wait && select != -1)
                        {
                            if(allies[select].getAgro())
                            {
                                allies[select].select(2);
                            } else {
                                allies[select].walk(2);
                                allies[select].setState(2);
                                direction = 2;
                                mouvement = true;
                                wait = true;
                            }
                        }
                        break;
                    case SDLK_LEFT:
                        if(physical_frame && !wait && select != -1)
                        {
                            if(allies[select].getAgro())
                            {
                                allies[select].select(3);
                            } else {
                                allies[select].walk(3);
                                allies[select].setState(3);
                                direction = 3;
                                mouvement = true;
                                wait = true;
                            }
                        }
                        break;
                    case SDLK_SPACE:
                        if(select != -1)
                        {
                            if(!allies[select].getAgro())
                            {
                                allies[select].switchMode();
                                allies[select].select(2);
                                allies[select].setState(5);
                            } else {
                                for(int i = 0 ; i < ennemis.size() ; i++)
                                {
                                    allies[select].attaquer(ennemis[i]);
                                }
                                allies[select].switchMode();
                            }
                        }
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
                            found = false;
                            for(int i = 0 ; i < allies.size() ; i++)
                            {
                                if(xmouse/64 == allies[i].getCoord().x/64 && ymouse/64 == allies[i].getCoord().y/64)
                                {
                                    select = i;
                                    found = true;
                                }
                                allies[i].deselectPerso();
                            }
                            if(found) {
                                allies[select].selectPerso();
                            } else {
                                select = -1;
                            }
                            break;
                        case SDL_BUTTON_RIGHT:
                            SDL_GetMouseState(&xmouse,&ymouse);
                            ennemis[0].deplacer(xmouse/64, ymouse/64);
                            break;
                    }
                }
        }

        SDL_RenderPresent(ecran);
        SDL_Delay(1000./60.);
    }

    desallouer_tab_2D(tab, lig);

    //Quitter SDL
    TTF_CloseFont(fontText);
    TTF_Quit();
    SDL_DestroyTexture(mageTexture);
    SDL_DestroyTexture(fond);
    SDL_DestroyRenderer(ecran);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
