#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "fonctions_SDL.h"
#include "fonctions_fichier.h"

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
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
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
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
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
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
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

void Personnage::attack(Personnage &cible, int** map, int lig, int col)
{
    int** mapTemp = m_spell.spellGrid(map, lig, col, m_pos[0], m_pos[1])[m_facing];
    for (unsigned x = 0; x < col; x++)
    {
        for (unsigned y = 0; y < lig; y++)
        {
            if (mapTemp[x][y] == 1 && cible.m_pos[0] == x && cible.m_pos[1] == y)
            {
                cible.recevoirDegats(m_spell.getDegats());
            }
        }
    }

    desallouer_tab_2D(mapTemp, col);
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
        if(m_pos[1] < 0 || mapTerrain[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[1]++;
        }
        break;
    case 1:
        m_pos[1]++;
        if(m_pos[1] > lig-1 || mapTerrain[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[1]--;
        }
        break;
    case 2:
        m_pos[0]++;
        if(m_pos[0] > col-1 || mapTerrain[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[0]--;
        }
        break;
    case 3:
        m_pos[0]--;
        if(m_pos[0] < 0 || mapTerrain[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[0]++;
        }
        break;
    }
    setState(direction + 1);
    setWait(5);
    desallouer_tab_2D(mapTerrain, col);
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

int Personnage::getFacing() const
{
    return m_facing;
}

int Personnage::getFacingMax() const
{
    return m_spell.getCycle();
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

void Personnage::setFacing(int** map, int lig, int col)
{
    m_facing = (m_facing + 1) % m_spell.getCycle();

    int i = 0;
    bool found = false;
    int** mapTemp = allouer_tab_2DInt(lig, col);
    while (!found || i >= getFacingMax())
    {
        mapTemp = spellGrid(map, lig, col)[m_facing];
        for (unsigned x = 0; x < col; x++)
        {
            for (unsigned y = 0; y < lig; y++)
            {
                if (mapTemp[x][y] == 1)
                {
                    found = true;
                }
            }
        }

        if (!found)
        {
            m_facing = (m_facing + 1) % m_spell.getCycle();
            i++;
        }
    }
    
    desallouer_tab_2D(mapTemp, col);

    setWait(10);
}

void Personnage::decreaseWait()
{
    if (m_wait > 0) {
        m_wait--;
    }
}

vector<int**> Personnage::spellGrid(int** map, int lig, int col)
{
    return m_spell.spellGrid(map, lig, col, m_pos[0], m_pos[1]);;
}

Personnage::~Personnage() 
{
}