#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include "fonctions_SDL.h"
#include "spell.h"

class Personnage
{
public:
    Personnage(int pv, int ac, vector<Spell> spells, string nom, int team);
    Personnage(int pv, int ac, vector<Spell> spells, string nom, int team, int x, int y);
    Personnage(Personnage const& copie);
    void takeDamage(int dmg, int attackRoll);
    void attack(Personnage &cible, const MaptabP *map, int attackRoll);
    int getBonusAttack() const;
    void activateInEffect();
    void decreaseMana();
    SDL_Rect afficherDegats(int texteW, int texteH);
    void deplacer(int x, int y);
    void walk(int direction, MaptabP *map);
    void heal(int qteHeal);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    int getHurt() const;
    int getSpellDmg();
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
    void setFacing(const MaptabP *map, int dir);
    void decreaseWait();
    void newTurn();
    void endTurn();
    int getSelectedSpell() const;
    int getMaxSpell() const;
    void selectSpell(int select);
    vector<MaptabP> spellGrid(const MaptabP *map);

private:
    int m_hp;
    int m_maxHp;
    int m_ac;
    int m_mana;
    string m_name;
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
    int m_bonusAtt;
    int m_bonusDmg;
};

#endif // PERSONNAGE_H_INCLUDED
