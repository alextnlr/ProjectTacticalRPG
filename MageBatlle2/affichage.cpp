#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "affichage.h"
#include "personnage.h"
#include "spell.h"

using namespace std;

Affichage::Affichage()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur init SDL: " << SDL_GetError() << endl;
        SDL_Quit();
    }

    if(TTF_Init() < 0)
    {
        cout << "Erreur init TTF: " << TTF_GetError() << endl;
        TTF_Quit();
    }

    m_widthWindow = 22*64;
    m_heightWindow = 10*64;
    m_window = SDL_CreateWindow("MageBattle",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,m_widthWindow,m_heightWindow,SDL_WINDOW_RESIZABLE);
    if(m_window == NULL) {
        cout << "Erreur creation fenetre: " << SDL_GetError() << endl;
        SDL_Quit();
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_background = charger_image("pavage.bmp",m_renderer);
    if(m_background == NULL) {
        cout << "Erreur chargement fond: " << SDL_GetError() << endl;
    }

    Uint8 r = 128, g = 160, b = 128;
    m_blueMageTexture = charger_image_transparente("MageM.bmp",m_renderer, r, g, b);
    m_redMageTexture = charger_image_transparente("MageMEvil.bmp",m_renderer, r, g, b);
    if(m_redMageTexture == NULL || m_blueMageTexture == NULL) {
        cout << "Erreur chargement texture: " << SDL_GetError() << endl;
    }

    m_font16 = TTF_OpenFont("prstartk.ttf",16);
    if(!m_font16) {
        cout << "Erreur du chargement de la font" << endl;
    }

    m_rectPos.x = 0;
    m_rectPos.y = 0;
    m_rectPos.w = 64;
    m_rectPos.h = 64;

    m_physicalFrame = true;
    m_frameNum = 0;

    setRectTerrain(16,10);
}

void Affichage::setRectTerrain(int nbColonnes, int nbLignes)
{
    int blocW, blocH;
    SDL_QueryTexture(m_background, NULL, NULL, &blocW, &blocH);

    SDL_Rect bloc;
    for(unsigned i = 0 ; i < nbColonnes*nbLignes ; i++) {
        bloc.x = i%nbColonnes*blocW/nbColonnes;
        bloc.y = i/nbColonnes*blocH/nbLignes;
        bloc.w = blocW/nbColonnes;
        bloc.h = blocH/nbLignes;
        m_rectTerrain.push_back(bloc);
    }
}

void Affichage::clearRenderer() const
{
    SDL_RenderClear(m_renderer);
}

void Affichage::displayRenderer() const
{
    SDL_RenderPresent(m_renderer);
    SDL_Delay(1000. / 60.);
}

void Affichage::setFrame()
{
    if(SDL_GetTicks() / m_TICK_INTERVALLE > m_frameNum) {
        m_frameNum = SDL_GetTicks() / m_TICK_INTERVALLE;
        m_physicalFrame = true;
    } else {
        m_physicalFrame = false;
    }
}

void Affichage::displayTerrain(int** mapTerrain, int lig, int col)
{
    for(int i = 0 ; i < lig ; i++) {
        m_rectPos.y = i*64;
        for(int j = 0 ; j < col ; j++) {
            m_rectPos.x = j*64;
            SDL_RenderCopy(m_renderer, m_background, &m_rectTerrain[mapTerrain[i][j]], &m_rectPos);
        }
    }
}

void Affichage::displayCharacters(vector<Personnage> &allies, vector<Personnage> &ennemies)
{
    for(int i = 0 ; i < ennemies.size() ; i++)
    {
        if (ennemies[i].estVivant())
        {
            SDL_Rect rectPos = ennemies[i].getCoord();
            SDL_Rect rectFrame = ennemies[i].afficherPersoBarre(m_physicalFrame);
            SDL_RenderCopy(m_renderer, m_redMageTexture, &rectFrame, &rectPos);
            if (ennemies[i].getHurt()) {
                SDL_Color color = { 0,0,0,0 };
                char dgt[3];
                sprintf_s(dgt, "%i", ennemies[i].getHurt());
                SDL_Texture* text = charger_texte(dgt, m_renderer, m_font16, color);
                int texteW, texteH;
                SDL_QueryTexture(text, NULL, NULL, &texteW, &texteH);
                SDL_Rect rectText = ennemies[i].afficherDegats(texteW, texteH);
                SDL_RenderCopy(m_renderer, text, NULL, &rectText);
            }
        }
    }

    for(int i = 0 ; i < allies.size() ; i++)
    {
        if (allies[i].estVivant())
        {
            SDL_Rect rectPos = allies[i].getCoord();
            SDL_Rect rectFrame = allies[i].afficherPersoBarre(m_physicalFrame);
            SDL_RenderCopy(m_renderer, m_blueMageTexture, &rectFrame, &rectPos);
            if (allies[i].getState() >= 0)
            {
                SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(m_renderer, &rectPos);
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            }
            if (allies[i].getHurt()) {
                SDL_Color color = { 0,0,0,0 };
                char dgt[3];
                sprintf_s(dgt, "%i", allies[i].getHurt());
                SDL_Texture* text = charger_texte(dgt, m_renderer, m_font16, color);
                int texteW, texteH;
                SDL_QueryTexture(text, NULL, NULL, &texteW, &texteH);
                SDL_Rect rectText = allies[i].afficherDegats(texteW, texteH);
                SDL_RenderCopy(m_renderer, text, NULL, &rectText);
            }
            allies[i].decreaseWait();
            if (allies[i].getWait() == 0 && allies[i].getState() >= 0 && m_physicalFrame)
            {
                allies[i].setState(0);
            }
        }
    }
}

void Affichage::displayInfoCard(vector<Personnage> &allies, vector<Personnage> &ennemies, int xmouse, int ymouse)
{
    m_found = false;
    for(int i = 0 ; i < allies.size() ; i++)
    {
        if(xmouse/64 == allies[i].getCoord().x/64 && ymouse/64 == allies[i].getCoord().y/64)
        {
            if(xmouse < 256 && ymouse < 128)
            {
                createInfoCard(allies[i], 1);
            } else {
                createInfoCard(allies[i], 1);
            }
            m_found = true;
        }
    }

    for(int i = 0 ; i < ennemies.size() ; i++)
    {
        if(xmouse/64 == ennemies[i].getCoord().x/64 && ymouse/64 == ennemies[i].getCoord().y/64)
        {
            if(xmouse < 256 && ymouse < 128)
            {
                createInfoCard(ennemies[i], 1);
            } else {
                createInfoCard(ennemies[i], 0);
            }
            m_found = true;
        }
    }


    for (int i = 0 ; i < allies.size() ; i++)
    {
        if(allies[i].getState() >= 0 && !m_found)
        {
            if(xmouse < 256 && ymouse < 128)
            {
                createInfoCard(allies[i], 1);
            } else {
                if(allies[i].getCoord().x < 256 && allies[i].getCoord().y < 128)
                {
                    createInfoCard(allies[i], 1);
                } else {
                    createInfoCard(allies[i], 0);
                }
            }
        }
    }
}

void Affichage::createInfoCard(Personnage perso, int pos) {
    if (perso.estVivant())
    {
        SDL_SetRenderDrawColor(m_renderer, 160, 82, 45, 255);
        SDL_Rect rectCard;
        rectCard.x = pos * 18 * 64;
        rectCard.y = 0;
        rectCard.w = 256;
        rectCard.h = 128;
        SDL_RenderFillRect(m_renderer, &rectCard);
        SDL_SetRenderDrawColor(m_renderer, 139, 69, 19, 255);
        SDL_RenderDrawRect(m_renderer, &rectCard);

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

        SDL_Color color = { 0,0,0,0 };
        char hp[10];
        sprintf_s(hp, "%i / %i", perso.getHp(), perso.getMaxHp());
        SDL_Texture* hpStr = charger_texte(hp, m_renderer, m_font16, color);
        int texteW, texteH;
        SDL_QueryTexture(hpStr, NULL, NULL, &texteW, &texteH);

        SDL_Rect textePos;
        textePos.x = rectCard.x + (rectCard.w - texteW) / 2;
        textePos.y = 60;
        textePos.w = texteW;
        textePos.h = texteH;

        SDL_RenderCopy(m_renderer, hpStr, NULL, &textePos);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_Rect UnderBar;
        UnderBar.w = rectCard.w * 2 / 3;
        UnderBar.h = 20;
        UnderBar.x = rectCard.x + (rectCard.w - UnderBar.w) / 2;
        UnderBar.y = 90;
        SDL_RenderFillRect(m_renderer, &UnderBar);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(m_renderer, &UnderBar);

        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
        SDL_Rect Bar;
        Bar.x = UnderBar.x;
        Bar.y = UnderBar.y;
        Bar.w = UnderBar.w * perso.getHp() / perso.getMaxHp();
        Bar.h = UnderBar.h;
        SDL_RenderFillRect(m_renderer, &Bar);

        SDL_Texture* nom = charger_texte(perso.getName(), m_renderer, m_font16, color);
        SDL_QueryTexture(nom, NULL, NULL, &texteW, &texteH);

        SDL_Rect nom_pos;
        nom_pos.x = rectCard.x + (rectCard.w - texteW) / 2;
        nom_pos.y = 25;
        nom_pos.w = texteW;
        nom_pos.h = texteH;

        SDL_RenderCopy(m_renderer, nom, NULL, &nom_pos);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    }
}

SDL_Renderer* Affichage::getRenderer() const
{
    return m_renderer;
}

TTF_Font* Affichage::getFontDmg() const
{
    return m_font16;
}

bool Affichage::getPhysicalFrame() const
{
    return m_physicalFrame;
}

void Affichage::desallouer()
{
    TTF_CloseFont(m_font16);
    SDL_DestroyTexture(m_redMageTexture);
    SDL_DestroyTexture(m_blueMageTexture);
    SDL_DestroyTexture(m_background);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();
}
