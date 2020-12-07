#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <iostream>
#include "Character.h"

using namespace std;

class Terrain
{
public:
    Terrain();
    Terrain(const string mapName);
    MaptabP getMapInt();
    ~Terrain();
private:
    MaptabP m_map;
};

#endif // TERRAIN_H_INCLUDED
