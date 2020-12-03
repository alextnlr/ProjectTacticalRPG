#include "spell.h"

using namespace std;

Spell::Spell()
{
    m_nom_sort = "Spark";
    m_dgt = 5;
    m_type = 0;
    m_cost = 0;
    m_cycle = 4;
    m_ineffect = 1;
    m_outeffect = 0;
    m_powerineffect = 1;
    m_powerouteffect = 0;
}

Spell::Spell(string nom, int dgt, int type, int cost)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case 2:
        m_cycle = 1;
        break;
    default:
        m_cycle = 4;
        break;
    }
    m_ineffect = 0;
    m_outeffect = 0;
    m_powerineffect = 0;
    m_powerouteffect = 0;
}

Spell::Spell(string nom, int dgt, int type, int cost, int ineffect, int outeffect, int powerineffect, int powerouteffect)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case 2:
        m_cycle = 1;
        break;
    default:
        m_cycle = 4;
        break;
    }
    m_ineffect = ineffect;
    m_outeffect = outeffect;
    m_powerineffect = powerineffect;
    m_powerouteffect = powerouteffect;
}

Spell::Spell(Spell const& copie)
{
    m_nom_sort = copie.m_nom_sort;
    m_dgt = copie.m_dgt;
    m_type = copie.m_type;
    m_cost = copie.m_cost;
    m_cycle = copie.m_cycle;
    m_ineffect = copie.m_ineffect;
    m_outeffect = copie.m_outeffect;
    m_powerineffect = copie.m_powerineffect;
    m_powerouteffect = copie.m_powerouteffect;
}

string Spell::getName() const
{
    return m_nom_sort;
}

int Spell::getCycle() const
{
    return m_cycle;
}

int Spell::getDegats() const
{
    return m_dgt;
}

int Spell::getCost() const
{
    return m_cost;
}

int Spell::activateInEffect() const
{
    return m_ineffect;
}

int Spell::getPowerInEffect() const
{
    return m_powerineffect;
}

vector<MaptabP> Spell::spellGrid(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid;
    spellGrid.clear();
    switch (m_type) {
    case 0:
        spellGrid = spellGridType0(map, posCastx, posCasty);
        break;
    case 1:
        spellGrid = spellGridType1(map, posCastx, posCasty);
        break;
    case 2:
        spellGrid = spellGridType2(map, posCastx, posCasty);
        break;
    default:
        spellGrid.push_back(allocateInt(map->lig, map->col));
        break;
    }
    return spellGrid;
}

vector<MaptabP> Spell::spellGridType0(const MaptabP *map, int posCastx, int posCasty)
{

    vector<MaptabP> spellGrid0;

    for (unsigned i = 0; i < 4; i++)
    {
        spellGrid0.push_back(allocateInt(map->lig, map->col));
    }

    if (posCasty - 1 >= 0 && map->mapInt[posCastx][posCasty - 1] == 0)
    {
        spellGrid0[0].mapInt[posCastx][posCasty - 1] = 1;
    }
    if (posCastx + 1 < map->col && map->mapInt[posCastx + 1][posCasty] == 0)
    {
        spellGrid0[1].mapInt[posCastx + 1][posCasty] = 1;
    }
    if (posCasty + 1 < map->lig && map->mapInt[posCastx][posCasty + 1] == 0)
    {
        spellGrid0[2].mapInt[posCastx][posCasty + 1] = 1;
    }
    if (posCastx - 1 >= 0 && map->mapInt[posCastx - 1][posCasty] == 0)
    {
        spellGrid0[3].mapInt[posCastx - 1][posCasty] = 1;
    }

    return spellGrid0;
}

vector<MaptabP> Spell::spellGridType1(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid1;
    for (unsigned i = 0; i < 4; i++)
    {
        spellGrid1.push_back(allocateInt(map->lig, map->col));
    }

    int k = 1;
    while (posCasty - k >= 0 && map->mapInt[posCastx][posCasty - k] == 0)
    {
        spellGrid1[0].mapInt[posCastx][posCasty - k] = 1;
        k++;
    }
    k = 1;
    while (posCastx + k < map->col && map->mapInt[posCastx + k][posCasty] == 0)
    {
        spellGrid1[1].mapInt[posCastx + k][posCasty] = 1;
        k++;
    }
    k = 1;
    while (posCasty + k < map->lig && map->mapInt[posCastx][posCasty + k] == 0)
    {
        spellGrid1[2].mapInt[posCastx][posCasty + k] = 1;
        k++;
    }
    k = 1;
    while (posCastx - k >= 0 && map->mapInt[posCastx - k][posCasty] == 0)
    {
        
        spellGrid1[3].mapInt[posCastx - k][posCasty] = 1;
        k++;
    }

    return spellGrid1;
}

vector<MaptabP> Spell::spellGridType2(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid2;
    spellGrid2.clear();
    spellGrid2.push_back(allocateInt(map->lig, map->col));
    spellGrid2[0].mapInt[posCastx][posCasty] = 1;
    return spellGrid2;
}

