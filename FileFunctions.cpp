#include <iostream>
#include "FileFunctions.h"

using namespace std;

MapcharP allocateChar(int n, int m) 
{
    MapcharP map;
    map.lig = n; 
    map.col = m;

    map.mapInt = new char*[m];

    for (unsigned i = 0; i < m; i++)
    {
        map.mapInt[i] = new char[n];
    }

    return map;
}

void deallocate(MapcharP &map)
{
    for (unsigned i = 0; i < map.col; i++)
    {
        delete[] map.mapInt[i];
    }
    delete[] map.mapInt;
}

MaptabP allocateInt(int n, int m)
{
    MaptabP map;
    map.lig = n; 
    map.col = m;

    map.mapInt = new int*[map.col];

    for (unsigned i = 0; i < m; i++)
    {
        map.mapInt[i] = new int[n];
    }

    for (unsigned x = 0; x < m; x++)
    {
        for (unsigned y = 0; y < n; y++)
        {
            map.mapInt[x][y] = 0;
        }
    }

    return map;
}

void deallocate(MaptabP *map)
{
        for (unsigned i = 0; i < map->col; i++)
        {
            delete[] map->mapInt[i];
        }
        delete[] map->mapInt;
}

void readMap(const MaptabP *map) 
{
    for (int y = 0; y < map->lig; y++)
    {
        for (int x = 0; x < map->col; x++)
        {
            cout << map->mapInt[x][y] << " ";
        }
        cout << endl;
    }
}

void sizeFile(const string nomFichier, int* nbLig, int* nbCol) {
    FILE* fichier = nullptr;
    fopen_s(&fichier,nomFichier.c_str(),"r");
    if(fichier == nullptr) {
        cout << "Echec de l'ouverture" << endl;
        exit(EXIT_FAILURE);
    }

    char chaine[40] = "";
    int col = 0;
    int lig = 0;

    while(fgets(chaine, 40, fichier) != NULL) {
        lig++;
        for(unsigned i = 0 ; i < 40 ; i++) {
            if(isgraph(chaine[i])) {col++;}
        }
        if(col > *nbCol) { *nbCol = col; }
        col = 0;
        for(unsigned j = 0 ; j<40 ; j++) {
            chaine[j] = ' ';
        }
    }
    *nbLig = lig;

    fclose(fichier);
}

MaptabP readFile(const string nomFichier) {

    FILE* fichier = nullptr;
    fopen_s(&fichier,nomFichier.c_str(),"r");
    if(fichier == nullptr) {
        cout << "Echec de l'ouverture" << endl;
        exit(EXIT_FAILURE);
    }

    int n=0, m=0;
    sizeFile(nomFichier, &n, &m);
    MapcharP map = allocateChar(n,m);
    char chaine[40] = " ";

    for(unsigned i = 0 ; i < n ; i++) {
        fgets(chaine, 40, fichier);
        for(unsigned j = 0 ; j < m ; j++) {
            if(isgraph(chaine[j])) {map.mapInt[j][i] = chaine[j];}
        }
        for(unsigned j = 0 ; j<40 ; j++) {
            chaine[j] = ' ';
        }
    }

    MaptabP mapInt = transfoCharToInt(map);
    deallocate(map);
    return mapInt;
}

MaptabP& transfoCharToInt(MapcharP& tabChar)
{
    MaptabP tabInt = allocateInt(tabChar.lig, tabChar.col);

    for (int i = 0; i < tabChar.col; i++) {
        for (int j = 0; j < tabChar.lig; j++) {
            tabInt.mapInt[i][j] = tabChar.mapInt[i][j] - '0';
        }
    }
    return tabInt;
}