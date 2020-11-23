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

    m_select = -1;
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

void Affichage::displayCharacters(vector<Personnage> allies, vector<Personnage> ennemies)
{
    for(int i = 0 ; i < ennemies.size() ; i++)
    {
        ennemies[i].afficherPersoBarre(m_renderer, m_physicalFrame);
        ennemies[i].afficherDegats(m_renderer, m_font16);
    }

    for(int i = 0 ; i < allies.size() ; i++)
    {
        allies[i].afficherPersoBarre(m_renderer, m_physicalFrame);
        allies[i].afficherDegats(m_renderer, m_font16);
    }
}

void Affichage::displayInfoCard(vector<Personnage> allies, vector<Personnage> ennemies, int xmouse, int ymouse)
{
    m_found = false;
    for(int i = 0 ; i < allies.size() ; i++)
    {
        if(xmouse/64 == allies[i].getCoord().x/64 && ymouse/64 == allies[i].getCoord().y/64)
        {
            if(xmouse < 256 && ymouse < 128)
            {
                allies[i].afficherInfos(m_renderer, m_font16, 1);
            } else {
                allies[i].afficherInfos(m_renderer, m_font16, 0);
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
                ennemies[i].afficherInfos(m_renderer, m_font16, 1);
            } else {
                ennemies[i].afficherInfos(m_renderer, m_font16, 0);
            }
            m_found = true;
        }
    }
    if(m_select >= 0 && !m_found)
    {
        if(xmouse < 256 && ymouse < 128)
        {
            allies[m_select].afficherInfos(m_renderer, m_font16, 1);
        } else {
            if(allies[m_select].getCoord().x < 256 && allies[m_select].getCoord().y < 128)
            {
                allies[m_select].afficherInfos(m_renderer, m_font16, 1);
            } else {
                allies[m_select].afficherInfos(m_renderer, m_font16, 0);
            }
        }
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

void Affichage::desallouer()
{
    TTF_CloseFont(m_font16);
    SDL_DestroyTexture(m_redMageTexture);
    SDL_DestroyTexture(m_blueMageTexture);
}
