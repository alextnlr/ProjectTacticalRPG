#include "Character.h"

using namespace std;

Character::Character(int pv, int ac , vector<Spell> spells, string nom, int team, int x, int y)
{
    m_maxHp = pv; //The maximum amount of damage the character can take
    m_hp = m_maxHp; //The current one
    m_ac = ac; //The Armor Class of the character, to hit, a character must roll the ac of the target or higher
    m_mvt = 6; //The number of unit the character can move in a turn
    m_spells.push_back(Spell()); //The base Spell "spark"
    for (Spell& spell : spells)
    {
        m_spells.push_back(spell);
    }
    m_team = team; //Red or blue
    m_mana = 5; //The ressource use to cast spell
    m_name = nom; //The name will be displayed on the info card
    m_pos[0] = x; //The current position in unit of the character
    m_pos[1] = y;
    m_hurt = false; //To know if the program have to display a number above the character
    m_dgtAnim = 0; //The number to display
    m_frameIdle = 0; //The number he must display
    m_state = -1;
    m_wait = false; //Cooldown of every action
    m_facing = 0; //For the spells
    m_end = false; //To know if the character can act
    m_selectedSpell = 0;
    m_bonusAtt = 5; //The bonus to the dice roll
    m_bonusDmg = 0; //The bonus to the damages of the spell used
    m_status = StatusList();
}

Character::Character(Character const& copie)
{
    m_hp = copie.m_hp;
    m_maxHp = copie.m_maxHp;
    m_ac = copie.m_ac;
    m_mvt = 6;
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
    if (attackRoll >= getAc() || dmg == 0) //Check if the attack roll is higher than the ac
    {
        m_hp -= dmg; //Reducing current hp

        if (m_hp < 0)
        {
            m_hp = 0;
        }
        else
        {
            activateOutEffect(effect, power); //If the attack spell has an effect
        }

        m_hurt = dmg;
    }
}

void Character::attack(Character &cible, const MaptabP *map, int attackRoll)
{
    if (cible.isAlive())
    {
        MaptabP mapTemp = m_spells[m_selectedSpell].spellGrid(map, m_pos[0], m_pos[1])[m_facing];
        if (mapTemp.mapInt[cible.m_pos[0]][cible.m_pos[1]] == 1) //Checking if the target is within the spell range
        {
            cible.takeDamage(m_spells[m_selectedSpell].getDegats() + getBonusDamage(), attackRoll + getBonusAttack(), 
                m_spells[m_selectedSpell].activateOutEffect(), m_spells[m_selectedSpell].getPowerOutEffect());
        }
        deallocate(&mapTemp);
    }
}

bool Character::canAttack()
{
    return m_mana-m_spells[m_selectedSpell].getCost() >= 0;
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
    return m_status.getTerrain(); //To know wich type of terrain unit he's on
}

bool* Character::getStatus()
{
    return m_status.getStatusActive();
}

void Character::activateInEffect() //To activate the bonus effect of a spell
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
        m_status.cleanAll();
        break;
    default:
        break;
    }
}

void Character::activateOutEffect(spellOutEffect effect, int power) //Same with the negative ones
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
        m_status.cleanAll();
        break;
    default:
        break;
    }
}

void Character::decreaseMana() //Used to decrease the mana of the user depending on the spell selected
{
    if (m_mana > 0)
    {
        m_mana -= m_spells[m_selectedSpell].getCost();
    }
}

SDL_Rect Character::getDmgDisplayer(int textW, int textH)
{

    SDL_Rect texte_pos; //This Rect will be used to know the position and size of the number
    texte_pos.x = m_pos[0]*64+16;
    texte_pos.y = m_pos[1]*64-m_dgtAnim;
    texte_pos.w = textW;
    texte_pos.h = textH;

    if(m_dgtAnim > 20) //After 20 phy-frames it reboot
    {
        m_hurt = false;
        m_dgtAnim = 0;
    } else {
        m_dgtAnim++; //Used to count the number of physical frames the animation has been on
    }

    return texte_pos;
}

void Character::setPosition(int x, int y) //Used in tests
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Character::walk(int direction, MaptabP *map)
{
    if (m_mvt > 0) //Check if there is any movement left
    {
        switch (direction)
        {
        case 0: //Up
            m_pos[1]--;
            if (m_pos[1] < 0 || map->mapInt[m_pos[0]][m_pos[1]] < 0) //Checking for collisions
            {
                m_pos[1]++;
            }
            break;
        case 1: //Down
            m_pos[1]++;
            if (m_pos[1] > map->lig - 1 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
            {
                m_pos[1]--;
            }
            break;
        case 2: //Right
            m_pos[0]++;
            if (m_pos[0] > map->col - 1 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
            {
                m_pos[0]--;
            }
            break;
        case 3: //Left
            m_pos[0]--;
            if (m_pos[0] < 0 || map->mapInt[m_pos[0]][m_pos[1]] < 0)
            {
                m_pos[0]++;
            }
            break;
        }
        m_status.setTerrain(map->mapInt[m_pos[0]][m_pos[1]]); //Refresh the terrain
        setState(direction + 1); //Set the direction for the animations
        m_mvt--; //Decrease movement by one
        setWait(5);
    }
}

void Character::checkTerrain(const MaptabP *map)
{
    m_status.setTerrain(map->mapInt[m_pos[0]][m_pos[1]]); //Used to know the first unit he's on
}

void Character::updateStatus()
{
    m_status.decreaseTimerAll();
    m_status.resetAll();
}

void Character::heal(int qteHeal)
{
    m_hp += qteHeal;

    if(m_hp > m_maxHp) //Cannot go above the max
    {
        m_hp = m_maxHp;
    }
}

SDL_Rect Character::getCoord() const
{
    SDL_Rect rect; //Return a Rect of the position in pixels with the size
    rect.x = m_pos[0]*64;
    rect.y = m_pos[1]*64;
    rect.w = 64;
    rect.h = 64;

    return rect;
}

bool Character::isAlive() const
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

int Character::getFacingMax() const //Used to know how many possibilities for the spell - if there is more than 4
{
    return m_spells[m_selectedSpell].getCycle();
}

string Character::getSpellName(int no)
{
    return m_spells[no].getName(); //For the display of the menu
}

void Character::recoverMana(int nbMana)
{
    m_mana += nbMana;
    if (m_mana > 5) //Never more than 5
    {
        m_mana = 5;
    }
}

SDL_Rect Character::getFrame(bool phy_frame)
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
    m_mvt = 6;
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