#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "fonctions_SDL.h"

using namespace std;

Personnage::Personnage(int pv, Spell spell, char* nom)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spell = spell;
    m_ptsMagie = 100;
    m_nom = nom;
    m_pos[0] = 0;
    m_pos[1] = 0;
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_mouvement = false;
    m_wait = false;
}

Personnage::Personnage(int pv, Spell spell, char* nom, int x, int y)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spell = spell;
    m_ptsMagie = 100;
    m_nom = nom;
    m_pos[0] = x;
    m_pos[1] = y;
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_mouvement = false;
    m_wait = false;
}

Personnage::Personnage(Personnage const& copie)
{
    m_vie = copie.m_vie;
    m_vieMax = copie.m_vieMax;
    m_spell = copie.m_spell;
    m_ptsMagie = copie.m_ptsMagie;
    m_nom = copie.m_nom;
    m_pos[0] = copie.m_pos[0];
    m_pos[1] = copie.m_pos[1];
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_mouvement = false;
    m_wait = false;
}

void Personnage::recevoirDegats(int nbDegats)
{
    m_vie -= nbDegats;

    if(m_vie < 0)
    {
        m_vie = 0;
    }

    m_hurt = nbDegats;
}

bool Personnage::attaquer(Personnage &cible)
{
    if(m_cible[0] == cible.m_pos[0] && m_cible[1] == cible.m_pos[1])
    {
        cible.recevoirDegats(m_spell.getDegats());
        return true;
    } else {
        return false;
    }
}

SDL_Rect Personnage::afficherDegats(int texteW, int texteH)
{

    SDL_Rect texte_pos;
    texte_pos.x = m_pos[0]*64+16;
    texte_pos.y = m_pos[1]*64-m_dgtAnim;
    texte_pos.w = texteW;
    texte_pos.h = texteH;

    if(m_dgtAnim > 20)
    {
        m_hurt = false;
        m_dgtAnim = 0;
    } else {
        m_dgtAnim++;
    }

    return texte_pos;
}

void Personnage::deplacer(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Personnage::walk(int direction, int** mapTerrain, int lig, int col)
{
    switch(direction)
    {
    case 0:
        m_pos[1]--;
        if(m_pos[1] < 0 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[1]++;
        }
        break;
    case 1:
        m_pos[1]++;
        if(m_pos[1] > lig-1 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[1]--;
        }
        break;
    case 2:
        m_pos[0]++;
        if(m_pos[0] > col-1 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[0]--;
        }
        break;
    case 3:
        m_pos[0]--;
        if(m_pos[0] < 0 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[0]++;
        }
        break;
    }
    setState(direction + 1);
    setWait(5);
}

void Personnage::heal(int qteHeal)
{
    m_vie += qteHeal;

    if(m_vie > m_vieMax)
    {
        m_vie = m_vieMax;
    }
}

SDL_Rect Personnage::getCoord() const
{
    SDL_Rect rect;
    rect.x = m_pos[0]*64;
    rect.y = m_pos[1]*64;
    rect.w = 64;
    rect.h = 64;

    return rect;
}

bool Personnage::estVivant() const
{
    return m_vie>0;
}

void Personnage::afficherEtat() const
{
    cout << m_nom << " PV = " << m_vie << endl;
}

void Personnage::afficherSelect(SDL_Renderer* renderer) const
{
    SDL_Rect rect;
    rect.x = m_cible[0]*64;
    rect.y = m_cible[1]*64;
    rect.w = 64;
    rect.h = 64;

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

bool Personnage::getAgro() const
{
    return m_agro;
}

int Personnage::getHurt() const
{
    return m_hurt;
}

int Personnage::getState() const
{
    return m_state;
}

int Personnage::getHp() const
{
    return m_vie;
}

int Personnage::getMaxHp() const
{
    return m_vieMax;
}

char* Personnage::getName() const
{
    return m_nom;
}

int Personnage::getWait() const
{
    return m_wait;
}

void Personnage::switchMode()
{
    if(m_agro)
    {
        m_agro = false;
    } else {
        m_agro = true;
    }
}

SDL_Rect Personnage::afficherPersoBarre(bool phy_frame)
{
    SDL_Rect Frame;
    int trueFrame = m_frameIdle/3;
    Frame.x = 31*trueFrame;
    if (m_state <= 0) {
        Frame.y = 0;
    }
    else
    {
        Frame.y = 31*m_state;
    }
    Frame.w = 31;
    Frame.h = 31;

    if(phy_frame)
    {
        m_frameIdle ++;
        if(m_frameIdle >= 4*3)
        {
            m_frameIdle = 0;
        }
    }

    return Frame;
}

void Personnage::setState(int state)
{
    m_state = state;
}

void Personnage::setWait(int temps)
{
    m_wait = temps;
}

void Personnage::decreaseWait()
{
    if (m_wait > 0) {
        m_wait--;
    }
}

void Personnage::desallouer()
{
    
}
