#ifndef SORT_H_INCLUDED
#define SORT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "fonctions_fichier.h"
#include "fonctions_SDL.h"

using namespace std;

typedef enum {Cac, Line, Self, Shock, Cone} spellType;
typedef enum {NullIn, Heal, Focus, ShieldUp, AttackUp, PrecisionUp, Clean} spellInEffect;
typedef enum {NullOut, ShieldDown, AttackDown, PrecisionDown, HealOther, CleanOther} spellOutEffect;

class Spell
{
public:
    Spell();
    Spell(string nom, string dgt, spellType type, int cost);
    Spell(string nom, string dgt, spellType type, int cost, spellInEffect ineffect, int powerineffect);
    Spell(string nom, string dgt, spellType type, int cost, spellOutEffect outeffect, int powerouteffect);
    Spell(string nom, string dgt, spellType type, int cost, spellInEffect ineffect, int powerineffect, spellOutEffect outeffect, int powerouteffect);
    Spell(Spell const& copie);
    string getName() const;
    int getCycle() const;
    int getDegats();
    int getCost() const;
    spellInEffect activateInEffect() const;
    spellOutEffect activateOutEffect() const;
    int getPowerInEffect() const;
    int getPowerOutEffect() const;
    void decryptDmg(std::string diceString, int& nbDice, int& dmgDice, int& bonusDmg);
    vector<MaptabP> spellGrid(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridTypeCac(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridTypeLine(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridTypeSelf(const MaptabP *map, int posCastx, int posCasty);
    vector<MaptabP> spellGridTypeShock(const MaptabP* map, int posCastx, int posCasty);
    vector<MaptabP> spellGridTypeCone(const MaptabP* map, int posCastx, int posCasty);
private:
    string m_nom_sort;
    string m_dgt;
    spellType m_type;
    int m_cost;
    int m_cycle;
    spellInEffect m_ineffect;
    spellOutEffect m_outeffect;
    int m_powerineffect;
    int m_powerouteffect;
};

#endif // SORT_H_INCLUDED
