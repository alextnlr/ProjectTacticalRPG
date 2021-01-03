#include "Fight.h"

using namespace std;

Fight::Fight()
{
    m_wait = 0;
    m_player = rand()%2;
}

vector<Character> Fight::createCharacter(const MaptabP *map)
{
    vector<Spell> spellList = {
        Spell("Artemis", "2d6+2", Line, 1, PrecisionUp, 3) ,
        Spell("Ares", "3d10+0", Cac, 2, AttackUp, 3, ShieldDown, 2),
        Spell("Poseidon", "1d8+4", Cone, 2, ShieldDown, 4),
        Spell("Zeus", "2d6+3", Line, 2, AttackDown, 4),
        Spell("Ades", "1d6+3", Shock, 2, Heal, 15, AttackDown, 2),
        Spell("Nyx", "1d6+0", Shock, 1, PrecisionDown, 4),
        Spell("Aphrodite", "0d0+0", Self, 2, Heal, 25),
        Spell("Athena", "2d6+3", Cac, 2, ShieldUp, 2),
        Spell("Hephaestus", "2d4+5", Cone, 1, ShieldDown, 2),
        Spell("Hermes", "1d4+5", Line, 1, PrecisionUp, 3),
        Spell("Dionysos", "0d0+0", Self, 1, AttackUp, 5),
        Spell("Demeter", "0d0+0", Self, 1, Clean, 1),
        Spell("Hestia", "0d0+0", Cac, 1, HealOther, 15),
        Spell("Nemesis", "0d0+0", Shock, 2, Clean, 1, CleanOther, 1),
        Spell("Apollo", "1d6+2", Line, 0, Focus, 2)
    };

    vector<string> name = {"Achille", "Belleph", "Heracles", "Jason", "OEdipe", "Persee", "Thesee", "Ulysse", "Ajax", "Amphi",
                            "Antigone", "Ariane", "Danaos", "Dedale", "Hector", "Leda", "Minos", "Nestor"};

    vector<Character> characters;
    vector<Spell> spellListTemp;
    
    vector<int> aled = {-1 , -1, -1};

    for (unsigned i = 0; i < 8; i++)
    {
        MaptabP mapcolli = createColli(characters, map, i);
        aled = {-1, -1, -1};
        for (unsigned j = 0; j < 3; j++)
        {
            int spellRand = rand() % spellList.size();
            auto truc = find(begin(aled), end(aled), spellRand);
            while (truc != end(aled))
            {
                spellRand = rand() % spellList.size();
                truc = find(begin(aled), end(aled), spellRand);
            }
            aled[j] = spellRand;
            spellListTemp.push_back(spellList[spellRand]);
        }

        int x = ((map->col-1) * (i % 2)) - (rand() % 3);
        int y = rand() % map->lig;

        while (mapcolli.mapInt[abs(x)][y] < 0)
        {
            x = ((map->col-1) * (i % 2)) - (rand() % 3);
            y = rand() % map->lig;
        }
        
        characters.push_back(Character(rand() % 8 + 25, rand() % 4 + 11, spellListTemp, name[rand() % name.size()], i % 2, abs(x), y));

        spellListTemp.clear();
        deallocate(&mapcolli);
    }

    for (Character &onechar : characters)
    {
        onechar.checkTerrain(map);
    }

    return characters;
}

int Fight::checkForVictory(vector<Character>& charac)
{
    bool blueVictory = true;
    bool redVictory = true;
    for (Character &onechara : charac)
    {
        if (onechara.getTeam() == 1 && onechara.isAlive())
        {
            blueVictory = false;
        }
        else if(onechara.getTeam() == 0 && onechara.isAlive())
        {
            redVictory = false;
        }
    }

    if (blueVictory)
    {
        return 0;
    }
    else if (redVictory)
    {
        return 1;
    }

    return -1;
}

void Fight::move(vector<Character> &characters, int dir, const MaptabP *map)
{
    for (unsigned i = 0; i < characters.size(); i++)
    {
        if (!characters[i].getWait() && characters[i].getState()>=0 && characters[i].getState() < 5)
        {
            MaptabP mapInt = createColli(characters, map, i);
            characters[i].walk(dir, &mapInt);
            deallocate(&mapInt);
        }
        else if (!m_wait && characters[i].getState() == 5)
        {
            if (dir == 1)
            {
                characters[i].selectSpell(1);
            }
            else if (dir == 0)
            {
                characters[i].selectSpell(-1);
            }
            setWait(10);
        }
        else if (!characters[i].getWait() && characters[i].getState() == 6)
        {
            characters[i].setFacing(map, dir);
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
                        if (persos[i].isAlive() && persos[i].getCoord().x / 64 == x && persos[i].getCoord().y / 64 == y)
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

void Fight::select(vector<Character> &characters, int xmouse, int ymouse)
{
        for (Character& onechara : characters)
        {
            onechara.setState(-1);
            if (onechara.getTeam() == m_player && xmouse / 64 == onechara.getCoord().x / 64 && ymouse / 64 == onechara.getCoord().y / 64 && onechara.getEnd() == false)
            {
                onechara.setState(0);
            }
        }
}

void Fight::deselect(vector<Character>& characters)
{
    for (Character &onechara : characters)
    {
        onechara.setState(-1);
    }
}

int Fight::getTeam() const
{
    return m_player;
}

bool Fight::cancel(vector<Character>& characters)
{
    if (!m_wait)
    {
        bool found = false;
        for (Character& onechara : characters)
        {
            if (onechara.getState() >= 0)
            {
                if (onechara.getState() < 5)
                {
                    onechara.setState(-1);
                }
                else
                {
                    onechara.setState(onechara.getState() - 1);
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
            else if (persos[i].getTeam() == m_player && persos[i].getState() == 5 && persos[i].canAttack())
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

void Fight::switchTeams(vector<Character> &characters)
{
    if (m_wait == 0)
    {
        m_player = (m_player + 1) % 2;
        deselect(characters);
        for (Character & onechara : characters)
        {
            onechara.newTurn();
            onechara.updateStatus();
        }
        setWait(20);
    }
}

void Fight::autoNewTurn(vector<Character> &characters)
{
    int nbInTeam = 0;
    for (Character & onechara : characters)
    {
        if (onechara.isAlive() && onechara.getTeam() == m_player)
        {
            nbInTeam++;
        }
    }

    int nbEndTurn = 0;
    for (Character & onechara : characters)
    {
        if (onechara.isAlive() && onechara.getEnd() && onechara.getTeam() == m_player)
        {
            nbEndTurn++;
        }
    }

    if (nbInTeam == nbEndTurn)
    {
        switchTeams(characters);
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