#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "terrain.h"
#include "fonctions_SDL.h"
#include "fonctions_fichier.h"

using namespace std;

Terrain::Terrain(SDL_Renderer* renderer, vector<Personnage> allies)
{
    taille_fichier("terrain.txt", &m_lig, &m_col);
    m_mapChar = lire_fichier("terrain.txt");
    m_map = transfoCharToInt(m_mapChar,m_lig,m_col);
    m_case = charger_image("pavage.bmp",renderer);

    int blocW, blocH;
    SDL_QueryTexture(m_case, NULL, NULL, &blocW, &blocH);

    SDL_Rect bloc;
    for(unsigned i = 0 ; i < 160 ; i++) {
        bloc.x = i%16*blocW/16;
        bloc.y = i/16*blocH/10;
        bloc.w = blocW/16;
        bloc.h = blocH/10;
        m_bloc.push_back(bloc);
    }

    m_blocPos.x = 0;
    m_blocPos.y = 0;
    m_blocPos.w = 64;
    m_blocPos.h = 64;

    Uint8 r = 128, g = 160, b = 128;
    m_mageTextureEvil = charger_image_transparente("MageMEvil.bmp",renderer, r, g, b);

    m_allies = allies;

    m_physical_frame = true;
    m_mouvement = false;
    m_direction = false;
    m_wait = false;
    m_found = false;

    m_frameNum = 0;
    m_select = -1;
}

Terrain::Terrain(SDL_Renderer* renderer, vector<Personnage> allies, char* nom_map, char* nom_case, int xcase, int ycase)
{
    taille_fichier(nom_map, &m_lig, &m_col);
    m_mapChar = lire_fichier(nom_map);
    m_map = transfoCharToInt(m_mapChar,m_lig,m_col);
    m_case = charger_image(nom_case,renderer);

    int blocW, blocH;
    SDL_QueryTexture(m_case, NULL, NULL, &blocW, &blocH);

    SDL_Rect bloc;
    for(unsigned i = 0 ; i < xcase*ycase ; i++) {
        bloc.x = i%xcase*blocW/xcase;
        bloc.y = i/xcase*blocH/ycase;
        bloc.w = blocW/xcase;
        bloc.h = blocH/ycase;
        m_bloc.push_back(bloc);
    }

    m_blocPos.x = 0;
    m_blocPos.y = 0;
    m_blocPos.w = 64;
    m_blocPos.h = 64;

    Uint8 r = 128, g = 160, b = 128;
    m_mageTextureEvil = charger_image_transparente("MageMEvil.bmp",renderer, r, g, b);

    m_allies = allies;

    m_physical_frame = true;
    m_mouvement = false;
    m_direction = false;
    m_wait = false;

    m_frameNum = 0;
    m_select = -1;
}

void Terrain::ajouterEnnemi(int pv, int dgt, char* nom, int posx, int posy)
{
    m_ennemis.push_back(Personnage(pv,dgt,nom,posx,posy,m_mageTextureEvil));
}

void Terrain::setFrame()
{
    if(SDL_GetTicks() / m_TICK_INTERVALLE > m_frameNum) {
        m_frameNum = SDL_GetTicks() / m_TICK_INTERVALLE;
        m_physical_frame = 1;
    } else {
        m_physical_frame = 0;
    }
}

void Terrain::afficherTerrain(SDL_Renderer* renderer)
{
    for(int i = 0 ; i < m_lig ; i++) {
        m_blocPos.y = i*64;
        for(int j = 0 ; j < m_col ; j++) {
            m_blocPos.x = j*64;
            SDL_RenderCopy(renderer, m_case, &m_bloc[m_map[i][j]], &m_blocPos);
        }
    }
}

void Terrain::afficherPersos(SDL_Renderer* renderer)
{
    for(int i = 0 ; i < m_ennemis.size() ; i++)
    {
        m_ennemis[i].afficherPersoBarre(renderer, m_physical_frame);
    }

    for(int i = 0 ; i < m_allies.size() ; i++)
    {
        m_allies[i].afficherPersoBarre(renderer, m_physical_frame);
    }
}

void Terrain::afficherDegats(SDL_Renderer* renderer, TTF_Font* fontText)
{
    for(int i = 0 ; i < m_ennemis.size() ; i++)
    {
        m_ennemis[i].afficherDegats(renderer, fontText);
    }
    for(int i = 0 ; i < m_allies.size() ; i++)
    {
        m_allies[i].afficherDegats(renderer, fontText);
    }
}

void Terrain::afficherAgro(SDL_Renderer* renderer)
{
    if(m_select >= 0)
    {
        if(m_allies[m_select].getAgro())
        {
            m_allies[m_select].afficherSelect(renderer);
        }
        if(m_allies[m_select].getAgro())
        {
            m_allies[m_select].setState(5);
        } else if(m_mouvement)
        {
            m_allies[m_select].setState(m_direction+1);
        } else
        {
            m_allies[m_select].setState(0);
        }
        m_allies[m_select].afficherRectSel(renderer);
    }
}

void Terrain::afficherInfos(SDL_Renderer* renderer, TTF_Font* fontText, int xmouse, int ymouse)
{
    if(m_select < 0)
        {
            for(int i = 0 ; i < m_allies.size() ; i++)
            {
                if(xmouse/64 == m_allies[i].getCoord().x/64 && ymouse/64 == m_allies[i].getCoord().y/64)
                {
                    if(xmouse < 256 && ymouse < 128)
                    {
                        m_allies[i].afficherInfos(renderer, fontText, 1);
                    } else {
                        m_allies[i].afficherInfos(renderer, fontText, 0);
                    }
                }
            }
            for(int i = 0 ; i < m_ennemis.size() ; i++)
            {
                if(xmouse/64 == m_ennemis[i].getCoord().x/64 && ymouse/64 == m_ennemis[i].getCoord().y/64)
                {
                    if(xmouse < 256 && ymouse < 128)
                    {
                        m_ennemis[i].afficherInfos(renderer, fontText, 1);
                    } else {
                        m_ennemis[i].afficherInfos(renderer, fontText, 0);
                    }
                }
            }
        }
        else
        {
            if(xmouse < 256 && ymouse < 128)
            {
                m_allies[m_select].afficherInfos(renderer, fontText, 1);
            } else {
                if(m_allies[m_select].getCoord().x < 256 && m_allies[m_select].getCoord().y < 128)
                {
                    m_allies[m_select].afficherInfos(renderer, fontText, 1);
                } else {
                    m_allies[m_select].afficherInfos(renderer, fontText, 0);
                }
            }
        }
}

void Terrain::switchMvtWait()
{
    if(m_physical_frame && m_frameNum%2) {
        m_mouvement = false;
        m_wait = false;
    }
}

void Terrain::deplacerSelect(int dir)
{
    if(m_physical_frame && !m_wait && m_select != -1)
        {
            if(m_allies[m_select].getAgro())
        {
            m_allies[m_select].select(dir);
        } else {
            m_allies[m_select].walk(dir);
            m_allies[m_select].setState(dir);
            m_direction = dir;
            m_mouvement = true;
            m_wait = true;
        }
    }
}

void Terrain::switchMode()
{
    if(m_select >= 0)
    {
        if(!m_allies[m_select].getAgro())
        {
            m_allies[m_select].switchMode();
            m_allies[m_select].select(2);
            m_allies[m_select].setState(5);
        } else {
            for(int i = 0 ; i < m_ennemis.size() ; i++)
            {
                m_allies[m_select].attaquer(m_ennemis[i]);
            }
            m_allies[m_select].switchMode();
        }
    }
}

void Terrain::select(int xmouse, int ymouse)
{
    m_found = false;
    for(int i = 0 ; i < m_allies.size() ; i++)
    {
        if(xmouse/64 == m_allies[i].getCoord().x/64 && ymouse/64 == m_allies[i].getCoord().y/64)
        {
            m_select = i;
            m_found = true;
        }
    }
    if(!m_found) {
        m_select = -1;
    }
}

void Terrain::deselect()
{
    m_select = -1;
}

void Terrain::desallouer()
{
    SDL_DestroyTexture(m_mageTextureEvil);
    SDL_DestroyTexture(m_case);
    desallouer_tab_2D(m_map, m_lig);
    desallouer_tab_2D(m_mapChar, m_lig);
    for(int i = m_ennemis.size()-1 ; i >= 0 ; i--)
    {
        m_ennemis[i].desallouer();
    }
    for(int i = m_allies.size()-1 ; i >= 0 ; i--)
    {
        m_allies[i].desallouer();
    }
}

