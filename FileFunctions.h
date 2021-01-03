#ifndef FONCTIONS_FICHIER_H_INCLUDED
#define FONCTIONS_FICHIER_H_INCLUDED

#include <string>
#include <vector>
#include <math.h>

typedef struct s_map {
	int lig; 
	int col;
	int** mapInt;
} MaptabP;

typedef struct s_mapChar {
	int lig;
	int col;
	char** mapInt;
} MapcharP;

MapcharP allocateChar(int n, int m);
void deallocate(MapcharP& map);
MaptabP allocateInt(int n, int m);
void deallocate(MaptabP* map);

void readMap(const MaptabP* map);

void sizeFile(std::string nomFichier, int* nbLig, int* nbCol);
MaptabP readFile(std::string nomFichier);

MaptabP& transfoCharToInt(MapcharP &tabChar);

#endif // FONCTIONS_FICHIER_H_INCLUDED
