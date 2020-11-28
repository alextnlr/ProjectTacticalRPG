#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "terrain.h"
#include "fonctions_SDL.h"
#include "fonctions_fichier.h"

using namespace std;

Terrain::Terrain()
{
    taille_fichier("terrain.txt", &m_lig, &m_col);
    m_mapChar = lire_fichier("terrain.txt");
    m_map = transfoCharToInt(m_mapChar,m_lig,m_col);
}

Terrain::Terrain(char* nom_map)
{
    taille_fichier(nom_map, &m_lig, &m_col);
    m_mapChar = lire_fichier(nom_map);
    m_map = transfoCharToInt(m_mapChar,m_lig,m_col);
}

int** Terrain::getMapInt() const 
{
    return m_map;
}

/*int** Terrain::getMapColli(vector<Personnage> allies, vector<Personnage> ennemies)
{
    for (unsigned i = 0; i < m_lig; i++)
    {
        for (unsigned j = 0; j < m_col; j++)
        {

        }
    }
}*/

int Terrain::getLig() const 
{
    return m_lig;
}

int Terrain::getCol() const
{
    return m_col;
}

void Terrain::desallouer()
{
    desallouer_tab_2D(m_map, m_lig);
    desallouer_tab_2D(m_mapChar, m_lig);
}

