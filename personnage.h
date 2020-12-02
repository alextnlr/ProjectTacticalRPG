#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "spell.h"

class Personnage
{
public:
    Personnage(int pv, Spell spell, char* nom, int team);
    Personnage(int pv, Spell spell, char* nom, int team, int x, int y);
    Personnage(Personnage const& copie);
    void recevoirDegats(int nbDegats);
    void attack(Personnage &cible, int** map, int lig, int col);
    void decreaseMana();
    SDL_Rect afficherDegats(int texteW, int texteH);
    void deplacer(int x, int y);
    void walk(int direction, int** mapTerrain, int lig, int col);
    void heal(int qteHeal);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    int getHurt() const;
    int getState() const;
    int getHp() const;
    int getMaxHp() const;
    int getMana() const;
    char* getName() const;
    int getTeam() const;
    int getWait() const;
    int getFacing() const;
    int getFacingMax() const;
    SDL_Rect afficherPersoBarre(bool phy_frame);
    void setState(int state);
    void setWait(int temps);
    void setFacing(int** map, int lig, int col);
    void decreaseWait();
    vector<int**> spellGrid(int** map, int lig, int col);
    ~Personnage();

private:
    int m_vie;
    int m_vieMax;
    int m_ptsMagie;
    char* m_nom;
    int m_pos[2];
    Spell m_spell;
    int m_hurt;
    int m_dgtAnim;
    int m_frameIdle;
    int m_state;
    int m_wait;
    int m_facing;
    int m_team;
};

#endif // PERSONNAGE_H_INCLUDED
