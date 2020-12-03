#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED

#include "personnage.h"

using namespace std;

class Combat
{
public:
    Combat();
    void move(vector<Personnage> &persos, int dir, const MaptabP *map);
    MaptabP createColli(vector<Personnage> &persos, const MaptabP *map, int num);
    void select(vector<Personnage> &persos, int xmouse, int ymouse);
    void deselect(vector<Personnage>& persos);
    int getTeam() const;
    void shiftAction(vector<Personnage>& persos, const MaptabP *map);
    void switchTeams(vector<Personnage> &persos);
    void autoNewTurn(vector<Personnage>& persos);
    void decreaseWait();
    void setWait(int wait);
private:
    int m_wait;
    int m_player;
};

#endif // COMBAT_H_INCLUDED
