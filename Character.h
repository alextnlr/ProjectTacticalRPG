#ifndef Character_H_INCLUDED
#define Character_H_INCLUDED

#include "FileFunctions.h"
#include "Spell.h"
#include "StatusList.h"

class Character
{
public:
    //Constructors
    Character(int pv, int ac, vector<Spell> spells, string nom, int team);
    Character(int pv, int ac, vector<Spell> spells, string nom, int team, int x, int y);
    Character(Character const& copie);

    //Simpler Getters
    int getAc();
    int getBonusAttack();
    int getBonusDamage();
    SDL_Rect getCoord() const;
    bool getEnd() const;
    int getFacing() const;
    int getFacingMax() const;
    int getHp() const;
    int getMaxHp() const;
    int getHurt() const;
    int getMana() const;
    string getName() const;
    int getSpellDmg();
    string getSpellName(int no);
    int getSelectedSpell() const;
    int getMaxSpell() const;
    bool* getStatus();
    int getState() const;
    int getTeam() const;
    TerrainEffect getTerrain();
    int getWait() const;

    //Transformative getters
    bool canAttack();
    SDL_Rect getFrame(bool phy_frame);
    SDL_Rect getDmgDisplayer(int textW, int textH);
    vector<MaptabP> spellGrid(const MaptabP* map);
    bool isAlive() const;

    //Actions and setters
    void takeDamage(int dmg, int attackRoll, spellOutEffect effect, int power);
    void activateInEffect();
    void activateOutEffect(spellOutEffect effect, int power);
    void updateStatus();
    void decreaseMana();
    void setPosition(int x, int y);
    void walk(int direction, MaptabP* map);
    void checkTerrain(const MaptabP* map);
    void attack(Character &cible, const MaptabP *map, int attackRoll);
    void heal(int qteHeal);
    void recoverMana(int nbMana);
    void setState(int state);
    void setWait(int temps);
    void setFacing(const MaptabP *map, int dir);
    void decreaseWait();
    void newTurn();
    void endTurn();
    void selectSpell(int select);
    

private:
    int m_hp;
    int m_maxHp;
    int m_ac;
    int m_mana;
    string m_name;
    int m_pos[2];
    vector<Spell> m_spells;
    StatusList m_status;
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

#endif // Character_H_INCLUDED
