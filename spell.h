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
    Spell(char* nom, int dgt, int type, int cost);
    Spell(Spell const& copie);
    int getDegats() const;
    int** attaque(int** obstacles);
private:
    char* m_nom_sort;
    int m_dgt;
    int m_type;
    int m_cost;
};

#endif // SORT_H_INCLUDED
