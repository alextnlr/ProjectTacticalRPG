#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "spell.h"

using namespace std;

class Terrain
{
public:
    Terrain();
    Terrain(char* nom_map);
    int** getMapInt() const;
    //int** getMapColli(vector<Personnage> allies, vector<Personnage> ennemies);
    int getLig() const;
    int getCol() const;
    void desallouer();
private:

    char** m_mapChar;
    int** m_map;
    int m_lig;
    int m_col;
};

#endif // TERRAIN_H_INCLUDED
