#include "Terrain.h"

using namespace std;

Terrain::Terrain()
{
    m_map = readFile("./Assets/terrain.txt");
}

Terrain::Terrain(const string mapName)
{
    m_map = readFile(mapName);
}

MaptabP Terrain::getMapInt() 
{
    return m_map;
}

Terrain::~Terrain()
{
    deallocate(&m_map);
}