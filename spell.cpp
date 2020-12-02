#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "spell.h"
#include "fonctions_fichier.h"

using namespace std;

Spell::Spell()
{
    m_nom_sort = (char*)"Spark";
    m_dgt = 5;
    m_type = 0;
    m_cost = 1;
    m_cycle = 4;
}

Spell::Spell(char* nom, int dgt, int type, int cost, int cycle)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    m_cycle = cycle;
}

Spell::Spell(Spell const& copie)
{
    m_nom_sort = copie.m_nom_sort;
    m_dgt = copie.m_dgt;
    m_type = copie.m_type;
    m_cost = copie.m_cost;
    m_cycle = copie.m_cycle;
}

int Spell::getCycle() const
{
    return m_cycle;
}

int Spell::getDegats() const
{
    return m_dgt;
}

vector<int**> Spell::spellGrid(int** map, int lig, int col, int posCastx, int posCasty)
{
    vector<int**> spellGrid;
    switch (m_type) {
    case 0:
        spellGrid = spellGridType0(map, lig, col, posCastx, posCasty);
        break;
    }
    return spellGrid;
}

vector<int**> Spell::spellGridType0(int** &map, int lig, int col, int posCastx, int posCasty) {

    vector<int**> spellGrid0;

    for (unsigned i = 0; i < 4; i++)
    {
        spellGrid0.push_back(allouer_tab_2DInt(lig, col));
    }

    if (posCasty - 1 >= 0 && map[posCastx][posCasty - 1] == 0)
    {
        spellGrid0[0][posCastx][posCasty - 1] = 1;
    }
    if (posCastx + 1 < col && map[posCastx + 1][posCasty] == 0)
    {
        spellGrid0[1][posCastx + 1][posCasty] = 1;
    }
    if (posCasty + 1 < lig && map[posCastx][posCasty + 1] == 0)
    {
        spellGrid0[2][posCastx][posCasty + 1] = 1;
    }
    if (posCastx - 1 >= 0 && map[posCastx - 1][posCasty] == 0)
    {
        spellGrid0[3][posCastx - 1][posCasty] = 1;
    }

    return spellGrid0;
}
