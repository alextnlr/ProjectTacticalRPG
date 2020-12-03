#include "personnage.h"

using namespace std;

Personnage::Personnage(int pv, vector<Spell> spells, string nom, int team)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spells.push_back(Spell());
    for (Spell &spell : spells)
    {
        m_spells.push_back(spell);
    }
    m_team = team;
    m_ptsMagie = 5;
    m_nom = nom;
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
}

Personnage::Personnage(int pv, vector<Spell> spells, string nom, int team, int x, int y)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spells.push_back(Spell());
    for (Spell& spell : spells)
    {
        m_spells.push_back(spell);
    }
    m_team = team;
    m_ptsMagie = 5;
    m_nom = nom;
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
}

Personnage::Personnage(Personnage const& copie)
{
    m_vie = copie.m_vie;
    m_vieMax = copie.m_vieMax;
    m_spells = copie.m_spells;
    m_team = copie.m_team;
    m_ptsMagie = copie.m_ptsMagie;
    m_nom = copie.m_nom;
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
}

void Personnage::recevoirDegats(int nbDegats)
{
    m_vie -= nbDegats;

    if(m_vie < 0)
    {
        m_vie = 0;
    }

    m_hurt = nbDegats;
}

void Personnage::attack(Personnage &cible, const MaptabP *map)
{
    MaptabP mapTemp = m_spells[m_selectedSpell].spellGrid(map, m_pos[0], m_pos[1])[m_facing];
    for (unsigned x = 0; x < map->col; x++)
    {
        for (unsigned y = 0; y < map->lig; y++)
        {
            if (mapTemp.mapInt[x][y] == 1 && cible.m_pos[0] == x && cible.m_pos[1] == y)
            {
                cible.recevoirDegats(m_spells[m_selectedSpell].getDegats());
            }
        }
    }
    deallocate(&mapTemp);
}

void Personnage::activateInEffect()
{
    switch (m_spells[m_selectedSpell].activateInEffect())
    {
    case 1:
        recoverMana(m_spells[m_selectedSpell].getPowerInEffect());
        break;
    case 2:
        heal(m_spells[m_selectedSpell].getPowerInEffect());
    default:
        break;
    }
}

void Personnage::decreaseMana()
{
    if (m_ptsMagie > 0)
    {
        m_ptsMagie -= m_spells[m_selectedSpell].getCost();
    }
}

SDL_Rect Personnage::afficherDegats(int texteW, int texteH)
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

void Personnage::deplacer(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Personnage::walk(int direction, MaptabP *map)
{
    switch(direction)
    {
    case 0:
        m_pos[1]--;
        if(m_pos[1] < 0 || map->mapInt[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[1]++;
        }
        break;
    case 1:
        m_pos[1]++;
        if(m_pos[1] > map->lig-1 || map->mapInt[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[1]--;
        }
        break;
    case 2:
        m_pos[0]++;
        if(m_pos[0] > map->col-1 || map->mapInt[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[0]--;
        }
        break;
    case 3:
        m_pos[0]--;
        if(m_pos[0] < 0 || map->mapInt[m_pos[0]][m_pos[1]] != 0)
        {
            m_pos[0]++;
        }
        break;
    }
    setState(direction + 1);
    setWait(5);
}

void Personnage::heal(int qteHeal)
{
    m_vie += qteHeal;

    if(m_vie > m_vieMax)
    {
        m_vie = m_vieMax;
    }
}

SDL_Rect Personnage::getCoord() const
{
    SDL_Rect rect;
    rect.x = m_pos[0]*64;
    rect.y = m_pos[1]*64;
    rect.w = 64;
    rect.h = 64;

    return rect;
}

bool Personnage::estVivant() const
{
    return m_vie>0;
}

int Personnage::getHurt() const
{
    return m_hurt;
}

int Personnage::getState() const
{
    return m_state;
}

int Personnage::getHp() const
{
    return m_vie;
}

int Personnage::getMaxHp() const
{
    return m_vieMax;
}

int Personnage::getMana() const
{
    return m_ptsMagie;
}

string Personnage::getName() const
{
    return m_nom;
}

int Personnage::getTeam() const
{
    return m_team;
}

bool Personnage::getEnd() const
{
    return m_end;
}

int Personnage::getWait() const
{
    return m_wait;
}

int Personnage::getFacing() const
{
    return m_facing;
}

int Personnage::getFacingMax() const
{
    return m_spells[m_selectedSpell].getCycle();
}

string Personnage::getSpellName(int no)
{
    return m_spells[no].getName();
}

void Personnage::recoverMana(int nbMana)
{
    m_ptsMagie += nbMana;
    if (m_ptsMagie > 5)
    {
        m_ptsMagie = 5;
    }
}

SDL_Rect Personnage::afficherPersoBarre(bool phy_frame)
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

void Personnage::setState(int state)
{
    m_state = state;
}

void Personnage::setWait(int temps)
{
    m_wait = temps;
}

void Personnage::setFacing(const MaptabP *map)
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

    setWait(10);
    deallocate(&mapTemp);
}

void Personnage::decreaseWait()
{
    if (m_wait > 0) {
        m_wait--;
    }
}

void Personnage::newTurn()
{
    m_end = false;
}

void Personnage::endTurn()
{
    m_end = true;
    setState(-1);
}

int Personnage::getSelectedSpell() const
{
    return m_selectedSpell;
}

int Personnage::getMaxSpell() const
{
    return m_spells.size();
}

void Personnage::selectSpell(int select)
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

vector<MaptabP> Personnage::spellGrid(const MaptabP *map)
{
    return m_spells[m_selectedSpell].spellGrid(map, m_pos[0], m_pos[1]);;
}