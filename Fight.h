#ifndef Fight_H_INCLUDED
#define Fight_H_INCLUDED

#include "Character.h"

using namespace std;

class Fight
{
public:
    Fight();
    vector<Character> createCharacter(const MaptabP *map);
    int checkForVictory(vector<Character>& charac);
    void move(vector<Character> &persos, int dir, const MaptabP *map);
    MaptabP createColli(vector<Character> &persos, const MaptabP *map, int num);
    void select(vector<Character> &persos, int xmouse, int ymouse);
    void deselect(vector<Character>& persos);
    int getTeam() const;
    bool cancel(vector<Character>& persos);
    int shiftAction(vector<Character>& persos, const MaptabP *map);
    void switchTeams(vector<Character> &persos);
    void autoNewTurn(vector<Character>& persos);
    void decreaseWait();
    void setWait(int wait);
private:
    int m_wait;
    int m_player;
};

#endif // Fight_H_INCLUDED
