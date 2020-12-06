#include "Fight.h"

using namespace std;

Fight::Fight()
{
    m_wait = 0;
    m_player = 0;
}

void Fight::move(vector<Character> &persos, int dir, const MaptabP *map)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        if (!persos[i].getWait() && persos[i].getState()>=0 && persos[i].getState() < 5)
        {
            MaptabP mapInt = createColli(persos, map, i);
            persos[i].walk(dir, &mapInt);
            deallocate(&mapInt);
        }
        else if (!m_wait && persos[i].getState() >= 0 && persos[i].getState() == 5)
        {
            if (dir == 1)
            {
                persos[i].selectSpell(1);
            }
            else if (dir == 0)
            {
                persos[i].selectSpell(-1);
            }
            setWait(10);
        }
        else if (!persos[i].getWait() && persos[i].getState() >= 0 && persos[i].getState() == 6)
        {
            persos[i].setFacing(map, dir);
        }
    }
}

MaptabP Fight::createColli(vector<Character> &persos, const MaptabP *map, int num)
{
    MaptabP colli = allocateInt(map->lig, map->col);
    
    for (unsigned x = 0; x < map->col; x++)
    {
        for (unsigned y = 0; y < map->lig; y++)
        {
            colli.mapInt[x][y] = map->mapInt[x][y];
            if (colli.mapInt[x][y] > 5)
            {
                colli.mapInt[x][y] = -1;
            }
            else
            {
                for (unsigned i = 0; i < persos.size(); i++)
                {
                    if (i != num) 
                    {
                        if (persos[i].estVivant() && persos[i].getCoord().x / 64 == x && persos[i].getCoord().y / 64 == y)
                        {
                            colli.mapInt[x][y] = -1;
                        }
                    }
                }
            }
        }
    }

    return colli;
}

void Fight::select(vector<Character> &persos, int xmouse, int ymouse)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        persos[i].setState(-1);
        if (persos[i].getTeam() == m_player && xmouse / 64 == persos[i].getCoord().x / 64 && ymouse / 64 == persos[i].getCoord().y / 64 && persos[i].getEnd() == false)
        {
            persos[i].setState(0);
        }
    }
}

void Fight::deselect(vector<Character>& persos)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        persos[i].setState(-1);
    }
}

int Fight::getTeam() const
{
    return m_player;
}

bool Fight::cancel(vector<Character>& persos)
{
    if (!m_wait)
    {
        bool found = false;
        for (Character& perso : persos)
        {
            if (perso.getState() >= 0)
            {
                if (perso.getState() < 5)
                {
                    perso.setState(-1);
                }
                else
                {
                    perso.setState(perso.getState() - 1);
                }

                setWait(20);
                found = true;
            }
        }
        return found;
    }
    return true;
}

int Fight::shiftAction(vector<Character>& persos, const MaptabP *map)
{
    if (m_wait == 0)
    {
        for (unsigned i = 0; i < persos.size(); i++)
        {
            if (persos[i].getTeam() == m_player && persos[i].getState() >= 0 && persos[i].getState() < 5)
            {
                persos[i].setState(5);
            }
            else if (persos[i].getTeam() == m_player && persos[i].getState() == 5)
            {
                persos[i].setState(6);
                persos[i].setFacing(map, 2);
            }
            else if (persos[i].getTeam() == m_player && persos[i].getState() == 6)
            {
                int attackRollFinal;
                if (persos[i].getSpellDmg() == 0)
                {
                    attackRollFinal = 0;
                }
                else
                {
                    int attackRoll = std::rand() % 20;
                    attackRollFinal = attackRoll + 1;
                }
                
                for (unsigned k = 0; k < persos.size(); k++)
                {
                    if (k != i)
                    {
                        persos[i].attack(persos[k], map, attackRollFinal);
                    }
                }
                persos[i].decreaseMana();
                persos[i].activateInEffect();
                persos[i].endTurn();

                if (attackRollFinal == 0)
                {
                    return attackRollFinal;
                }
                return attackRollFinal + persos[i].getBonusAttack();
            }

        }

        setWait(10);
    }

    return 0;
}

void Fight::switchTeams(vector<Character> &persos)
{
    if (m_wait == 0)
    {
        m_player = (m_player + 1) % 2;
        deselect(persos);
        for (Character & perso : persos)
        {
            perso.newTurn();
            perso.updateStatus();
        }
        setWait(20);
    }
}

void Fight::autoNewTurn(vector<Character> &persos)
{
    int nbInTeam = 0;
    for (Character & perso : persos)
    {
        if (perso.estVivant() && perso.getTeam() == m_player)
        {
            nbInTeam++;
        }
    }

    int nbEndTurn = 0;
    for (Character & perso : persos)
    {
        if (perso.estVivant() && perso.getEnd() && perso.getTeam() == m_player)
        {
            nbEndTurn++;
        }
    }

    if (nbInTeam == nbEndTurn)
    {
        switchTeams(persos);
    }
}

void Fight::decreaseWait()
{
    if (m_wait > 0)
    {
        m_wait--;
    }
}

void Fight::setWait(int wait)
{
    m_wait = wait;
}