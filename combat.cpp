#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"
#include "personnage.h"
#include "combat.h"

using namespace std;

Combat::Combat()
{
    m_wait = 0;
}

void Combat::move(vector<Personnage> &allies, vector<Personnage> &ennemies, bool physicalFrame, int dir, int** map, int lig, int col)
{
    for (unsigned i = 0; i < allies.size(); i++)
    {
        if (!allies[i].getWait() && allies[i].getState()>=0 && allies[i].getState() != 5)
        {
            allies[i].walk(dir, createColli(allies, ennemies, map, lig, col, i), lig, col);
        }
        else if (!allies[i].getWait() && allies[i].getState() >= 0 && allies[i].getState() == 5)
        {
            allies[i].setFacing(map, lig, col);
        }
    }
}

int** Combat::createColli(vector<Personnage> &allies, vector<Personnage> &ennemies, int** map, int lig, int col, int num)
{
    int** colli = allouer_tab_2DInt(lig, col);
    for (unsigned x = 0; x < col; x++)
    {
        for (unsigned y = 0; y < lig; y++)
        {
            if (map[x][y])
            {
                colli[x][y] = 1;
            }
            else
            {
                for (unsigned i = 0; i < allies.size(); i++)
                {
                    if (i != num) 
                    {
                        if (allies[i].estVivant() && allies[i].getCoord().x / 64 == x && allies[i].getCoord().y / 64 == y)
                        {
                            colli[x][y] = 1;
                        }
                    }
                }

                for (unsigned i = 0; i < ennemies.size(); i++)
                {
                    if (ennemies[i].estVivant() && ennemies[i].getCoord().x / 64 == x && ennemies[i].getCoord().y / 64 == y)
                    {
                        colli[x][y] = 1;
                    }
                }
            }
        }
    }

    return colli;
}

void Combat::select(vector<Personnage> &allies, int xmouse, int ymouse)
{
    for (unsigned i = 0; i < allies.size(); i++)
    {
        allies[i].setState(-1);
        if (xmouse / 64 == allies[i].getCoord().x / 64 && ymouse / 64 == allies[i].getCoord().y / 64)
        {
            allies[i].setState(0);
        }
    }
}

void Combat::shiftAction(vector<Personnage>& allies, vector<Personnage>& ennemies, int** map, int lig, int col)
{
    if (m_wait == 0)
    {
        for (unsigned i = 0; i < allies.size(); i++)
        {
            if (allies[i].getState() >= 0 && allies[i].getState() != 5)
            {
                allies[i].setState(5);
                allies[i].setFacing(map, lig, col);
            }
            else if (allies[i].getState() == 5)
            {
                for (Personnage& ennemy : ennemies)
                {
                    allies[i].attack(ennemy, map, lig, col);
                }

                allies[i].decreaseMana();
                allies[i].setState(0);
            }
        }

        setWait(10);
    }
}

void Combat::switchTeams(vector<Personnage> &allies, vector<Personnage> &ennemies)
{
    if (m_wait == 0)
    {
        vector<Personnage> temp = allies;
        allies.clear();
        allies = ennemies;
        ennemies.clear();
        ennemies = temp;
        temp.clear();
        setWait(20);
    }
}

void Combat::decreaseWait()
{
    if (m_wait > 0)
    {
        m_wait--;
    }
}

void Combat::setWait(int wait)
{
    m_wait = wait;
}