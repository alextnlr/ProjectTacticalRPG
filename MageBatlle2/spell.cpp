#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "spell.h"

using namespace std;

Spell::Spell()
{
    m_nom_sort = (char*)"Spark";
    m_dgt = 1;
    m_type = 0;
    m_cost = 1;
}

Spell::Spell(char* nom, int dgt, int type, int cost)
{
    m_nom_sort = nom;
    m_dgt = dgt;
    m_type = type;
    m_cost = cost;
}

Spell::Spell(Spell const& copie)
{
    m_nom_sort = copie.m_nom_sort;
    m_dgt = copie.m_dgt;
    m_type = copie.m_type;
    m_cost = copie.m_cost;
}

int Spell::getDegats() const
{
    return m_dgt;
}

/*int** Spell::attaque(int** obstacles, int posCast[2])
{
    switch (m_type) {
    case 0:
        break;
    }
}*/
