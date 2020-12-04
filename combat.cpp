#include "combat.h"

using namespace std;

Combat::Combat()
{
    m_wait = 0;
    m_player = 0;
}

void Combat::move(vector<Personnage> &persos, int dir, const MaptabP *map)
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

MaptabP Combat::createColli(vector<Personnage> &persos, const MaptabP *map, int num)
{
    MaptabP colli = allocateInt(map->lig, map->col);
    for (unsigned x = 0; x < map->col; x++)
    {
        for (unsigned y = 0; y < map->lig; y++)
        {
            if (map->mapInt[x][y] != 0)
            {
                colli.mapInt[x][y] = 1;
            }
            else
            {
                for (unsigned i = 0; i < persos.size(); i++)
                {
                    if (i != num) 
                    {
                        if (persos[i].estVivant() && persos[i].getCoord().x / 64 == x && persos[i].getCoord().y / 64 == y)
                        {
                            colli.mapInt[x][y] = 1;
                        }
                    }
                }
            }
        }
    }

    return colli;
}

void Combat::select(vector<Personnage> &persos, int xmouse, int ymouse)
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

void Combat::deselect(vector<Personnage>& persos)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        persos[i].setState(-1);
    }
}

int Combat::getTeam() const
{
    return m_player;
}

int Combat::shiftAction(vector<Personnage>& persos, const MaptabP *map)
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

void Combat::switchTeams(vector<Personnage> &persos)
{
    if (m_wait == 0)
    {
        m_player = (m_player + 1) % 2;
        deselect(persos);
        for (Personnage & perso : persos)
        {
            perso.newTurn();
        }
        setWait(20);
    }
}

void Combat::autoNewTurn(vector<Personnage> &persos)
{
    int nbInTeam = 0;
    for (Personnage & perso : persos)
    {
        if (perso.getTeam() == m_player)
        {
            nbInTeam++;
        }
    }

    int nbEndTurn = 0;
    for (Personnage & perso : persos)
    {
        if (perso.getEnd() && perso.getTeam() == m_player)
        {
            nbEndTurn++;
        }
    }

    if (nbInTeam == nbEndTurn)
    {
        switchTeams(persos);
    }
}

void Combat::decreaseWait()
{
    if (m_wait > 0)
    {
        m_wait--;
    }
}

void Combat::setWait(int wait)
{
    m_wait = wait;
}