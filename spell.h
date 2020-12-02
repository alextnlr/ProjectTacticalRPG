#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

using namespace std;

class Spell
{
public:
    Spell();
    Spell(char* nom, int dgt, int type, int cost, int cycle);
    Spell(Spell const& copie);
    int getCycle() const;
    int getDegats() const;
    vector<int**> spellGrid(int** map, int lig, int col, int posCastx, int posCasty);
    vector<int**> spellGridType0(int** &map, int lig, int col, int posCastx, int posCasty);
private:
    char* m_nom_sort;
    int m_dgt;
    int m_type;
    int m_cost;
    int m_cycle;
};

#endif // SORT_H_INCLUDED
