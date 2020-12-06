#include "spell.h"

using namespace std;

Spell::Spell()
{
    m_nom_sort = "Spark";
    m_dgt = "1d4+1";
    m_type = Cac;
    m_cost = 0;
    m_cycle = 4;
    m_ineffect = Focus;
    m_outeffect = NullOut;
    m_powerineffect = 1;
    m_powerouteffect = 0;
}

Spell::Spell(string nom, string dgt, spellType type, int cost)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case Self:
    case Shock:
        m_cycle = 1;
        break;
    default:
        m_cycle = 4;
        break;
    }
    m_ineffect = NullIn;
    m_outeffect = NullOut;
    m_powerineffect = 0;
    m_powerouteffect = 0;
}

Spell::Spell(string nom, string dgt, spellType type, int cost, spellInEffect ineffect, int powerineffect)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case Self:
    case Shock:
        m_cycle = 1;
        break;
    default:
        m_cycle = 4;
        break;
    }
    m_ineffect = ineffect;
    m_outeffect = NullOut;
    m_powerineffect = powerineffect;
    m_powerouteffect = 0;
}

Spell::Spell(string nom, string dgt, spellType type, int cost, spellOutEffect outeffect, int powerouteffect)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case Self:
    case Shock:
        m_cycle = 1;
        break;
    default:
        m_cycle = 4;
        break;
    }
    m_ineffect = NullIn;
    m_outeffect = outeffect;
    m_powerineffect = 0;
    m_powerouteffect = powerouteffect;
}

Spell::Spell(string nom, string dgt, spellType type, int cost, spellInEffect ineffect, int powerineffect, spellOutEffect outeffect, int powerouteffect)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
    switch (type)
    {
    case Self:
    case Shock:
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

int Spell::getDegats()
{
    int nbDice, dmgDice, bonusDmg;
    decryptDmg(m_dgt, nbDice, dmgDice, bonusDmg);
    int dmg = 0;
    for (unsigned i = 0; i < nbDice; i++)
    {
        int dmgRoll = std::rand() % dmgDice + 1;
        dmg += dmgRoll;
    }
    dmg += bonusDmg;
    
    return dmg;
}

int Spell::getCost() const
{
    return m_cost;
}

spellInEffect Spell::activateInEffect() const
{
    return m_ineffect;
}

spellOutEffect Spell::activateOutEffect() const
{
    return m_outeffect;
}

int Spell::getPowerInEffect() const
{
    return m_powerineffect;
}

int Spell::getPowerOutEffect() const
{
    return m_powerouteffect;
}

void Spell::decryptDmg(std::string diceString, int &nbDice, int &dmgDice, int &bonusDmg)
{
    int i = 0;
    string nbDiceStr = "";
    while (diceString[i] != 'd')
    {
        nbDiceStr += diceString[i];
        i++;
    }
    nbDice = stoi(nbDiceStr);
    i++;
    nbDiceStr = "";
    while (diceString[i] != '+')
    {
        nbDiceStr += diceString[i];
        i++;
    }
    dmgDice = stoi(nbDiceStr);
    i++;
    nbDiceStr = "";
    for (unsigned j = i; j < diceString.size(); j++)
    {
        nbDiceStr += diceString[i];
        i++;
    }
    bonusDmg = stoi(nbDiceStr);
}

vector<MaptabP> Spell::spellGrid(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid;
    spellGrid.clear();
    switch (m_type) {
    case Cac:
        spellGrid = spellGridTypeCac(map, posCastx, posCasty);
        break;
    case Line:
        spellGrid = spellGridTypeLine(map, posCastx, posCasty);
        break;
    case Self:
        spellGrid = spellGridTypeSelf(map, posCastx, posCasty);
        break;
    case Shock:
        spellGrid = spellGridTypeShock(map, posCastx, posCasty);
        break;
    default:
        spellGrid.push_back(allocateInt(map->lig, map->col));
        break;
    }
    return spellGrid;
}

vector<MaptabP> Spell::spellGridTypeCac(const MaptabP *map, int posCastx, int posCasty)
{

    vector<MaptabP> spellGrid0;

    for (unsigned i = 0; i < 4; i++)
    {
        spellGrid0.push_back(allocateInt(map->lig, map->col));
    }

    if (posCasty - 1 >= 0 && map->mapInt[posCastx][posCasty - 1] < 5)
    {
        spellGrid0[0].mapInt[posCastx][posCasty - 1] = 1;
    }
    if (posCastx + 1 < map->col && map->mapInt[posCastx + 1][posCasty] < 5)
    {
        spellGrid0[2].mapInt[posCastx + 1][posCasty] = 1;
    }
    if (posCasty + 1 < map->lig && map->mapInt[posCastx][posCasty + 1] < 5)
    {
        spellGrid0[1].mapInt[posCastx][posCasty + 1] = 1;
    }
    if (posCastx - 1 >= 0 && map->mapInt[posCastx - 1][posCasty] < 5)
    {
        spellGrid0[3].mapInt[posCastx - 1][posCasty] = 1;
    }

    return spellGrid0;
}

vector<MaptabP> Spell::spellGridTypeLine(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid1;
    for (unsigned i = 0; i < 4; i++)
    {
        spellGrid1.push_back(allocateInt(map->lig, map->col));
    }

    int k = 1;
    while (posCasty - k >= 0 && map->mapInt[posCastx][posCasty - k] < 5)
    {
        spellGrid1[0].mapInt[posCastx][posCasty - k] = 1;
        k++;
    }
    k = 1;
    while (posCastx + k < map->col && map->mapInt[posCastx + k][posCasty] < 5)
    {
        spellGrid1[2].mapInt[posCastx + k][posCasty] = 1;
        k++;
    }
    k = 1;
    while (posCasty + k < map->lig && map->mapInt[posCastx][posCasty + k] < 5)
    {
        spellGrid1[1].mapInt[posCastx][posCasty + k] = 1;
        k++;
    }
    k = 1;
    while (posCastx - k >= 0 && map->mapInt[posCastx - k][posCasty] < 5)
    {
        
        spellGrid1[3].mapInt[posCastx - k][posCasty] = 1;
        k++;
    }

    return spellGrid1;
}

vector<MaptabP> Spell::spellGridTypeSelf(const MaptabP *map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid2;
    spellGrid2.clear();
    spellGrid2.push_back(allocateInt(map->lig, map->col));
    spellGrid2[0].mapInt[posCastx][posCasty] = 1;
    return spellGrid2;
}

vector<MaptabP> Spell::spellGridTypeShock(const MaptabP* map, int posCastx, int posCasty)
{
    vector<MaptabP> spellGrid;
    spellGrid.clear();
    spellGrid.push_back(allocateInt(map->lig, map->col));
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (posCastx + x >= 0 && posCastx + x < map->col)
            {
                if (posCasty + y >= 0 && posCasty + y < map->lig)
                {
                    if (map->mapInt[posCastx+x][posCasty+y] < 5)
                    {
                        spellGrid.at(0).mapInt[posCastx + x][posCasty + y] = 1;
                    }
                }
            }
        }
    }
    return spellGrid;
}

