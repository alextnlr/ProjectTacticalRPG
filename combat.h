#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"

using namespace std;

class Combat
{
public:
    Combat();
    void move(vector<Personnage> &allies, vector<Personnage> &ennemies, bool physicalFrame, int dir, int** map, int lig, int col);
    int** createColli(vector<Personnage> &allies, vector<Personnage> &ennemies, int** map, int lig, int col, int num);
    void changeFacing(vector<Personnage>& allies, int direction);
    void select(vector<Personnage> &allies, int xmouse, int ymouse);
    void shiftAction(vector<Personnage>& allies, vector<Personnage>& ennemies, int** map, int lig, int col);
private:
};

#endif // COMBAT_H_INCLUDED
