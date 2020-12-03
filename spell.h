#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"

using namespace std;

class Spell
{
public:
    Spell();
    Spell(string nom, int dgt, int type, int cost);
    Spell(string nom, int dgt, int type, int cost, int ineffect, int outeffect, int powerineffect, int powerouteffect);
    Spell(Spell const& copie);
    string getName() const;
    int getCycle() const;
    int getDegats() const;
    int getCost() const;
    int activateInEffect() const;
    int getPowerInEffect() const;
    vector<MaptabP> spellGrid(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridType0(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridType1(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridType2(const MaptabP *map, int posCastx, int posCasty);
private:
    string m_nom_sort;
    int m_dgt;
    int m_type;
    int m_cost;
    int m_cycle;
    int m_ineffect;
    int m_outeffect;
    int m_powerineffect;
    int m_powerouteffect;
};

#endif // SORT_H_INCLUDED
