#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include "fonctions_SDL.h"
#include "spell.h"

class Personnage
{
public:
    Personnage(int pv, vector<Spell> spells, string nom, int team);
    Personnage(int pv, vector<Spell> spells, string nom, int team, int x, int y);
    Personnage(Personnage const& copie);
    void recevoirDegats(int nbDegats);
    void attack(Personnage &cible, const MaptabP *map);
    void activateInEffect();
    void decreaseMana();
    SDL_Rect afficherDegats(int texteW, int texteH);
    void deplacer(int x, int y);
    void walk(int direction, MaptabP *map);
    void heal(int qteHeal);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    int getHurt() const;
    int getState() const;
    int getHp() const;
    int getMaxHp() const;
    int getMana() const;
    string getName() const;
    int getTeam() const;
    bool getEnd() const;
    int getWait() const;
    int getFacing() const;
    int getFacingMax() const;
    string getSpellName(int no);
    void recoverMana(int nbMana);
    SDL_Rect afficherPersoBarre(bool phy_frame);
    void setState(int state);
    void setWait(int temps);
    void setFacing(const MaptabP *map);
    void decreaseWait();
    void newTurn();
    void endTurn();
    int getSelectedSpell() const;
    int getMaxSpell() const;
    void selectSpell(int select);
    vector<MaptabP> spellGrid(const MaptabP *map);

private:
    int m_vie;
    int m_vieMax;
    int m_ptsMagie;
    string m_nom;
    int m_pos[2];
    vector<Spell> m_spells;
    int m_hurt;
    int m_dgtAnim;
    int m_frameIdle;
    int m_state;
    int m_wait;
    int m_facing;
    int m_team;
    int m_selectedSpell;
    bool m_end;
};

#endif // PERSONNAGE_H_INCLUDED
