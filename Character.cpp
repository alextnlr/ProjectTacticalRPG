#include "Character.h"

using namespace std;

Character::Character(int pv, int ac, vector<Spell> spells, string nom, int team)
{
    m_maxHp = pv;
    m_hp = m_maxHp;
    m_ac = ac;
    m_spells.push_back(Spell());
    for (Spell &spell : spells)
    {
        m_spells.push_back(spell);
    }
    m_team = team;
    m_mana = 5;
    m_name = nom;
    m_pos[0] = 0;
    m_pos[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
    m_end = false;
    m_selectedSpell = 0;
    m_bonusAtt = 5;
    m_bonusDmg = 0;
    m_status = StatusList();
}

Character::Character(int pv, int ac , vector<Spell> spells, string nom, int team, int x, int y)
{
    m_maxHp = pv;
    m_hp = m_maxHp;
    m_ac = ac;
    m_spells.push_back(Spell());
    for (Spell& spell : spells)
    {
        m_spells.push_back(spell);
    }
    m_team = team;
    m_mana = 5;
    m_name = nom;
    m_pos[0] = x;
    m_pos[1] = y;
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
    m_end = false;
    m_selectedSpell = 0;
    m_bonusAtt = 5;
    m_bonusDmg = 0;
    m_status = StatusList();
}

Character::Character(Character const& copie)
{
    m_hp = copie.m_hp;
    m_maxHp = copie.m_maxHp;
    m_ac = copie.m_ac;
    m_spells = copie.m_spells;
    m_team = copie.m_team;
    m_mana = copie.m_mana;
    m_name = copie.m_name;
    m_pos[0] = copie.m_pos[0];
    m_pos[1] = copie.m_pos[1];
    m_hurt = false;
    m_dgtAnim = 0;
    m_frameIdle = 0;
    m_state = -1;
    m_wait = false;
    m_facing = 0;
    m_end = false;
    m_selectedSpell = 0;
    m_bonusAtt = copie.m_bonusAtt;
    m_bonusDmg = copie.m_bonusDmg;
    m_status = copie.m_status;
}

void Character::takeDamage(int dmg, int attackRoll, spellOutEffect effect, int power)
{
    if (attackRoll >= getAc())
    {
        m_hp -= dmg;

        if (m_hp < 0)
        {
            m_hp = 0;
        }
        else
        {
            activateOutEffect(effect, power);
        }

        m_hurt = dmg;
    }
}

void Character::attack(Character &cible, const MaptabP *map, int attackRoll)
{
    if (cible.estVivant())
    {
        MaptabP mapTemp = m_spells[m_selectedSpell].spellGrid(map, m_pos[0], m_pos[1])[m_facing];
        for (unsigned x = 0; x < map->col; x++)
        {
            for (unsigned y = 0; y < map->lig; y++)
            {
                if (mapTemp.mapInt[x][y] == 1 && cible.m_pos[0] == x && cible.m_pos[1] == y)
                {
                    cible.takeDamage(m_spells[m_selectedSpell].getDegats() + getBonusDamage(), attackRoll + getBonusAttack(), m_spells[m_selectedSpell].activateOutEffect(), m_spells[m_selectedSpell].getPowerOutEffect());
                }
            }
        }
        deallocate(&mapTemp);
    }
}

int Character::getBonusAttack()
{
    return m_bonusAtt + m_status.getPrecision();
}

int Character::getAc()
{
    return m_ac + m_status.getShield();
}

int Character::getBonusDamage()
{
    return m_bonusDmg + m_status.getArcana();
}

TerrainEffect Character::getTerrain()
{
    return m_status.getTerrain();
}

bool* Character::getStatus()
{
    return m_status.getStatusActive();
}

void Character::activateInEffect()
{
    switch (m_spells[m_selectedSpell].activateInEffect())
    {
    case Focus:
        recoverMana(m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case Heal:
        heal(m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case ShieldUp:
        m_status.setShieldUp(4, m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case AttackUp:
        m_status.setAttackUp(5, m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case PrecisionUp:
        m_status.setPrecisionUp(4, m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case Clean:
        m_status.resetAll();
        break;
    default:
        break;
    }
}

void Character::activateOutEffect(spellOutEffect effect, int power)
{
    switch (effect)
    {
    case ShieldDown:
        m_status.setShieldDown(4, power);
        break;
    case AttackDown:
        m_status.setAttackDown(5, power);
        break;
    case PrecisionDown:
        m_status.setPrecisionDown(4, power);
        break;
    case HealOther:
        heal(power);
        break;
    case CleanOther:
        m_status.resetAll();
        break;
    default:
        break;
    }
}

void Character::decreaseMana()
{
    if (m_mana > 0)
    {
        m_mana -= m_spells[m_selectedSpell].getCost();
    }
}

SDL_Rect Character::afficherDegats(int texteW, int texteH)
{

    SDL_Rect texte_pos;
    texte_pos.x = m_pos[0]*64+16;
    texte_pos.y = m_pos[1]*64-m_dgtAnim;
    texte_pos.w = texteW;
    texte_pos.h = texteH;

    if(m_dgtAnim > 20)
    {
        m_hurt = false;
        m_dgtAnim = 0;
    } else {
        m_dgtAnim++;
    }

    return texte_pos;
}

void Character::deplacer(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Character::walk(int direction, MaptabP *map)
{
    switch(direction)
    {
    case 0:
        m_pos[1]--;
        if(m_pos[1] < 0 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
        {
            m_pos[1]++;
        }
        break;
    case 1:
        m_pos[1]++;
        if(m_pos[1] > map->lig-1 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
        {
            m_pos[1]--;
        }
        break;
    case 2:
        m_pos[0]++;
        if(m_pos[0] > map->col-1 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
        {
            m_pos[0]--;
        }
        break;
    case 3:
        m_pos[0]--;
        if(m_pos[0] < 0 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
        {
            m_pos[0]++;
        }
        break;
    }
    m_status.setTerrain(map->mapInt[m_pos[0]][m_pos[1]]);
    setState(direction + 1);
    setWait(5);
}

void Character::updateStatus()
{
    m_status.decreaseTimerAll();
    m_status.resetAll();
}

void Character::heal(int qteHeal)
{
    m_hp += qteHeal;

    if(m_hp > m_maxHp)
    {
        m_hp = m_maxHp;
    }
}

SDL_Rect Character::getCoord() const
{
    SDL_Rect rect;
    rect.x = m_pos[0]*64;
    rect.y = m_pos[1]*64;
    rect.w = 64;
    rect.h = 64;

    return rect;
}

bool Character::estVivant() const
{
    return m_hp>0;
}

int Character::getHurt() const
{
    return m_hurt;
}

int Character::getSpellDmg()
{
    return m_spells[m_selectedSpell].getDegats();
}

int Character::getState() const
{
    return m_state;
}

int Character::getHp() const
{
    return m_hp;
}

int Character::getMaxHp() const
{
    return m_maxHp;
}

int Character::getMana() const
{
    return m_mana;
}

string Character::getName() const
{
    return m_name;
}

int Character::getTeam() const
{
    return m_team;
}

bool Character::getEnd() const
{
    return m_end;
}

int Character::getWait() const
{
    return m_wait;
}

int Character::getFacing() const
{
    return m_facing;
}

int Character::getFacingMax() const
{
    return m_spells[m_selectedSpell].getCycle();
}

string Character::getSpellName(int no)
{
    return m_spells[no].getName();
}

void Character::recoverMana(int nbMana)
{
    m_mana += nbMana;
    if (m_mana > 5)
    {
        m_mana = 5;
    }
}

SDL_Rect Character::afficherPersoBarre(bool phy_frame)
{
    SDL_Rect Frame;
    int trueFrame = m_frameIdle/3;
    Frame.x = 31*trueFrame;
    if (m_state <= 0) 
    {
        Frame.y = 0;
    }
    else if (m_state > 5)
    {
        Frame.y = 31 * 5;
    }
    else
    {
        Frame.y = 31*m_state;
    }
    
    Frame.w = 31;
    Frame.h = 31;

    if(phy_frame)
    {
        m_frameIdle ++;
        if(m_frameIdle >= 4*3)
        {
            m_frameIdle = 0;
        }
    }

    return Frame;
}

void Character::setState(int state)
{
    m_state = state;
}

void Character::setWait(int temps)
{
    m_wait = temps;
}

void Character::setFacing(const MaptabP *map, int dir)
{
    if (m_spells[m_selectedSpell].getCycle() == 4)
    {
        m_facing = dir;
    }
    else
    {
        m_facing = (m_facing + 1) % m_spells[m_selectedSpell].getCycle();

        int i = 0;
        bool found = false;
        MaptabP mapTemp = allocateInt(map->lig, map->col);
        while (!found || i >= getFacingMax())
        {
            mapTemp = spellGrid(map)[m_facing];
            for (unsigned x = 0; x < map->col; x++)
            {
                for (unsigned y = 0; y < map->lig; y++)
                {
                    if (mapTemp.mapInt[x][y] == 1)
                    {
                        found = true;
                    }
                }
            }

            if (!found)
            {
                m_facing = (m_facing + 1) % m_spells[m_selectedSpell].getCycle();
                i++;
            }
        }
        deallocate(&mapTemp);
    }

    setWait(10);
}

void Character::decreaseWait()
{
    if (m_wait > 0) {
        m_wait--;
    }
}

void Character::newTurn()
{
    m_end = false;
}

void Character::endTurn()
{
    m_end = true;
    setState(-1);
}

int Character::getSelectedSpell() const
{
    return m_selectedSpell;
}

int Character::getMaxSpell() const
{
    return m_spells.size();
}

void Character::selectSpell(int select)
{
    m_selectedSpell += select;
    if (m_selectedSpell < 0)
    {
        m_selectedSpell++;
    }
    else if (m_selectedSpell >= getMaxSpell())
    {
        m_selectedSpell--;
    }
}

vector<MaptabP> Character::spellGrid(const MaptabP *map)
{
    return m_spells[m_selectedSpell].spellGrid(map, m_pos[0], m_pos[1]);;
}