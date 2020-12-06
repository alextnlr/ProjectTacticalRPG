#include <iostream>
#include "Display.h"

using namespace std;

Display::Display()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur init SDL: " << SDL_GetError() << endl;
        SDL_Quit();
    }

    if(TTF_Init() < 0)
    {
        cout << "Erreur init TTF: " << TTF_GetError() << endl;
        TTF_Quit();
    }

    m_widthWindow = 22*64;
    m_heightWindow = 10*64;
    m_window = SDL_CreateWindow("MageBattle",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,m_widthWindow,m_heightWindow,SDL_WINDOW_RESIZABLE);
    if(m_window == NULL) {
        cout << "Erreur creation fenetre: " << SDL_GetError() << endl;
        SDL_Quit();
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    m_background = charger_image("Tiles.bmp",m_renderer);
    if(m_background == NULL) {
        cout << "Erreur chargement fond: " << SDL_GetError() << endl;
    }

    Uint8 r = 128, g = 160, b = 128;
    m_blueMageTexture = charger_image_transparente("MageM.bmp",m_renderer, r, g, b);
    m_redMageTexture = charger_image_transparente("MageMEvil.bmp",m_renderer, r, g, b);
    m_endMageTexture = charger_image_transparente("MageF.bmp", m_renderer, r, g, b);
    if(m_redMageTexture == NULL || m_blueMageTexture == NULL || m_endMageTexture == NULL) {
        cout << "Erreur chargement texture de mage: " << SDL_GetError() << endl;
    }

    m_statusMarker = charger_image_transparente("StatusMarker.bmp", m_renderer, r, g, b);
    m_infoCard = charger_image_transparente("InfoCard.bmp", m_renderer, r, g, b);
    m_menu = charger_image_transparente("Menu.bmp", m_renderer, r, g, b);
    m_selectCase = charger_image_transparente("SelectCadre.bmp", m_renderer, r, g, b);
    m_spellSelect = charger_image_transparente("SpellSelect.bmp", m_renderer, r, g, b);
    m_spellNoSelect = charger_image_transparente("SpellNoSelect.bmp", m_renderer, r, g, b);
    m_d20 = charger_image_transparente("d20.bmp", m_renderer, r, g, b);
    if (m_infoCard == NULL || m_menu == NULL) {
        cout << "Erreur chargement texture info Card: " << SDL_GetError() << endl;
    }
    m_underBar = charger_image_transparente("UnderBar.bmp", m_renderer, r, g, b);
    m_bar = charger_image_transparente("Bar.bmp", m_renderer, r, g, b);
    if (m_underBar == NULL || m_bar == NULL) {
        cout << "Erreur chargement texture bar: " << SDL_GetError() << endl;
    }
    m_mana = charger_image_transparente("Mana.bmp", m_renderer, r, g, b);
    m_blackMana = charger_image_transparente("BlackMana.bmp", m_renderer, r, g, b);
    if (m_mana == NULL || m_blackMana == NULL) {
        cout << "Erreur chargement texture mana: " << SDL_GetError() << endl;
    }

    m_font64 = TTF_OpenFont("prstartk.ttf", 64);
    m_font16 = TTF_OpenFont("prstartk.ttf", 16);
    m_font8 = TTF_OpenFont("prstartk.ttf", 12);
    if(!m_font16) {
        cout << "Erreur du chargement de la font" << endl;
    }

    m_rectPos.x = 0;
    m_rectPos.y = 0;
    m_rectPos.w = 64;
    m_rectPos.h = 64;

    m_physicalFrame = true;
    m_frameNum = 0;

    setRectTerrain(4,2);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_currentTeam = 0;
    m_timerShowTeam = 60;
    m_stockRoll = 0;
}

void Display::setRectTerrain(int nbColonnes, int nbLignes)
{
    int blocW, blocH;
    SDL_QueryTexture(m_background, NULL, NULL, &blocW, &blocH);

    SDL_Rect bloc;
    for(unsigned i = 0 ; i < nbColonnes*nbLignes ; i++) {
        bloc.x = i%nbColonnes*blocW/nbColonnes;
        bloc.y = i/nbColonnes*blocH/nbLignes;
        bloc.w = blocW/nbColonnes;
        bloc.h = blocH/nbLignes;
        m_rectTerrain.push_back(bloc);
    }
}

void Display::clearRenderer() const
{
    SDL_RenderClear(m_renderer);
}

void Display::displayRenderer() const
{
    SDL_RenderPresent(m_renderer);
    SDL_Delay(1000. / 60.);
}

void Display::setFrame()
{
    if(SDL_GetTicks() / m_TICK_INTERVALLE > m_frameNum) {
        m_frameNum = SDL_GetTicks() / m_TICK_INTERVALLE;
        m_physicalFrame = true;
    } else {
        m_physicalFrame = false;
    }
}

void Display::displayTerrain(const MaptabP *map)
{
    for(int i = 0 ; i < map->col ; i++) {
        m_rectPos.x = i*64;
        for(int j = 0 ; j < map->lig ; j++) {
            m_rectPos.y = j*64;
            SDL_RenderCopy(m_renderer, m_background, &m_rectTerrain[map->mapInt[i][j]], &m_rectPos);
        }
    }
}

void Display::displayCharacters(vector<Character> &persos)
{
    for(int i = 0 ; i < persos.size() ; i++)
    {
        if (persos[i].estVivant())
        {
            SDL_Rect rectPos = persos[i].getCoord();
            SDL_Rect rectFrame = persos[i].afficherPersoBarre(m_physicalFrame);

            if (persos[i].getState() >= 0)
            {
                SDL_RenderCopy(m_renderer, m_selectCase, NULL, &rectPos);
            }

            if (persos[i].getEnd())
            {
                SDL_RenderCopy(m_renderer, m_endMageTexture, &rectFrame, &rectPos);
            } 
            else if (!persos[i].getTeam())
            {
                SDL_RenderCopy(m_renderer, m_blueMageTexture, &rectFrame, &rectPos);
            }
            else
            {
                SDL_RenderCopy(m_renderer, m_redMageTexture, &rectFrame, &rectPos);
            }
            
            persos[i].decreaseWait();
            if (persos[i].getWait() == 0 && persos[i].getState() >= 0 && m_physicalFrame && persos[i].getState() < 5)
            {
                persos[i].setState(0);
            }
        }
    }
}

bool Display::displayDamages(vector<Character>& persos, int roll)
{
    bool found = false;
    for (Character & perso : persos)
    {
        if (perso.getHurt() && !roll) {
            SDL_Color color = { 0,0,0,255 };
            SDL_Texture* text = charger_texte(to_string(perso.getHurt()).c_str(), m_renderer, m_font16, color);
            int texteW, texteH;
            SDL_QueryTexture(text, NULL, NULL, &texteW, &texteH);
            SDL_Rect rectText = perso.afficherDegats(texteW, texteH);
            SDL_RenderCopy(m_renderer, text, NULL, &rectText);
            SDL_DestroyTexture(text);
            found = true;
        }
    }
    if (found)
    {
        return true;
    }
    return false;
}

bool Display::displayRoll(int &roll)
{
    if (roll)
    {
        if (!m_timerShowRoll)
        {
            m_timerShowRoll = 90;
        }
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
        SDL_Rect blackScreen{ 0,0,m_widthWindow,m_heightWindow };
        SDL_RenderFillRect(m_renderer, &blackScreen);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

        SDL_Texture* rollName;
        int width, height;
        SDL_Rect pos;

        pos.x = m_widthWindow / 2 - 256 / 2;
        pos.y = m_heightWindow / 2 - 256 / 2;
        pos.w = 256;
        pos.h = 256;
        SDL_RenderCopy(m_renderer, m_d20, NULL, &pos);

        SDL_Color color;
        color = { 255, 255, 255, 255 };
        if (m_timerShowRoll > 40)
        {
            int attackRoll = std::rand() % 20;
            if (m_timerShowRoll%4 == 0)
            {
                m_stockRoll = attackRoll + 1;
            }
            rollName = charger_texte(to_string(m_stockRoll).c_str(), m_renderer, m_font64, color);
        }
        else
        {
            rollName = charger_texte(to_string(roll).c_str(), m_renderer, m_font64, color);
        }

        SDL_QueryTexture(rollName, NULL, NULL, &width, &height);

        pos.x = m_widthWindow / 2 - width / 2;
        pos.y = m_heightWindow / 2 - height / 2;
        pos.w = width;
        pos.h = height;

        SDL_RenderCopy(m_renderer, rollName, NULL, &pos);

        SDL_DestroyTexture(rollName);

        m_timerShowRoll--;
        if (m_timerShowRoll == 0)
        {
            roll = 0;
        }
    }

    return (m_timerShowRoll > 0);
}

void Display::displayMenu(vector<Character> &persos)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        if (persos[i].getState() == 5)
        {
            vector<SDL_Texture*> text;
            SDL_Color color;
            for (unsigned j = 0; j < persos[i].getMaxSpell(); j++)
            {
                string textChar = "";
                textChar += persos[i].getSpellName(j);
                if (j == persos[i].getSelectedSpell())
                {
                    color = { 255, 215, 0, 255 };
                }
                else
                {
                    color = { 255, 255, 255, 255 };
                }
                text.push_back(charger_texte(textChar.c_str(), m_renderer, m_font8, color));
            }

            SDL_Rect posText;
            posText.x = persos[i].getCoord().x + 20 + 64;
            int textW, textH, maxTextW;

            SDL_QueryTexture(text[0], NULL, NULL, &maxTextW, &textH);
            for (unsigned j = 1; j < text.size(); j++)
            {
                SDL_QueryTexture(text[j], NULL, NULL, &textW, &textH);
                if (textW > maxTextW)
                {
                    maxTextW = textW;
                }
            }

            SDL_Rect posBackground;
            posBackground.w = maxTextW + 30;
            posBackground.h = (20)*text.size() + textH + 20;
            posBackground.x = posText.x - 15;
            posBackground.y = persos[i].getCoord().y - 20;

            SDL_RenderCopy(m_renderer, m_menu, NULL, &posBackground);

            for (unsigned j = 0 ; j < text.size() ; j++)
            {
                SDL_QueryTexture(text[j], NULL, NULL, &textW, &textH);
                posText.y = persos[i].getCoord().y + j*20;
                posText.w = textW;
                posText.h = textH;
                SDL_RenderCopy(m_renderer, text[j], NULL, &posText);
            }

            for (SDL_Texture* &textTexture : text)
            {
                SDL_DestroyTexture(textTexture);
            }
            text.clear();
        }
    }
    
}

void Display::displaySpellRange(vector<Character> &persos, const MaptabP *map)
{
    for (unsigned i = 0; i < persos.size(); i++)
    {
        if (persos[i].getState() == 6)
        {
            vector<MaptabP> spellGrid = persos[i].spellGrid(map);
            for (unsigned k = 0 ; k < persos[i].getFacingMax() ; k++)
            {
                for (int x = 0; x < map->col; x++)
                {
                    for (int y = 0; y < map->lig; y++)
                    {
                        if (spellGrid[k].mapInt[x][y] > 0)
                        {
                            SDL_Rect rectSpellGrid;
                            rectSpellGrid.x = x * 64;
                            rectSpellGrid.y = y * 64;
                            rectSpellGrid.w = 64;
                            rectSpellGrid.h = 64;
                            if (persos[i].getFacing() == k)
                            {
                                SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 70);
                                SDL_RenderFillRect(m_renderer, &rectSpellGrid);
                                SDL_RenderCopy(m_renderer, m_spellSelect, NULL, &rectSpellGrid);
                            }
                            else 
                            {
                                SDL_SetRenderDrawColor(m_renderer, 255, 215, 0, 50);
                                SDL_RenderFillRect(m_renderer, &rectSpellGrid);
                                SDL_RenderCopy(m_renderer, m_spellNoSelect, NULL, &rectSpellGrid);
                            }
                            
                            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

                            for (unsigned j = 0; j < persos.size(); j++)
                            {
                                if (i != j)
                                {
                                    if (persos[j].estVivant() && persos[j].getCoord().x == rectSpellGrid.x && persos[j].getCoord().y == rectSpellGrid.y)
                                    {
                                        double proba = (double)persos[j].getAc() - persos[i].getBonusAttack();
                                        proba = (21-proba)/20*100;
                                        string probaStr = to_string((int)proba);
                                        probaStr += "%";
                                        SDL_Color color = {0, 0, 0, 255};
                                        SDL_Texture* probaText = charger_texte(probaStr.c_str(), m_renderer, m_font8, color);
                                        int textW, textH;
                                        SDL_QueryTexture(probaText, NULL, NULL, &textW, &textH);
                                        SDL_Rect posText{ persos[j].getCoord().x+32-textW/2, persos[j].getCoord().y-textH+15, textW, textH };
                                        SDL_RenderCopy(m_renderer, probaText, NULL, &posText);
                                        SDL_DestroyTexture(probaText);
                                    }
                                }
                            }
                        }
                        
                    }
                }

            }
            for (int i = spellGrid.size()-1; i >= 0; i--)
            {
                deallocate(&spellGrid.at(i));
            }
            spellGrid.clear();
        }
    }
}

bool Display::displayTeam(int team, int roll)
{
    if (team != m_currentTeam && !roll)
    {
        m_timerShowTeam = 60;
        m_currentTeam = team;
    }
    if (m_timerShowTeam > 0)
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
        SDL_Rect blackScreen{0,0,m_widthWindow,m_heightWindow};
        SDL_RenderFillRect(m_renderer, &blackScreen);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

        SDL_Texture* teamName;
        int texteW, texteH;
        SDL_Rect pos;
        SDL_Color color;
        if (m_currentTeam == 0)
        {
            color = { 0, 0, 255, 255 };
            teamName = charger_texte("Blue Team Turn", m_renderer, m_font64, color);
            SDL_QueryTexture(teamName, NULL, NULL, &texteW, &texteH);
        }
        else
        {
            color = { 255, 0, 0, 255 };
            teamName = charger_texte("Red Team Turn", m_renderer, m_font64, color);
            SDL_QueryTexture(teamName, NULL, NULL, &texteW, &texteH);
        }

        pos.x = (m_widthWindow - texteW)/2;
        pos.y = m_heightWindow/2-texteH;
        pos.w = texteW;
        pos.h = texteH;

        SDL_RenderCopy(m_renderer, teamName, NULL, &pos);

        SDL_DestroyTexture(teamName);

        m_timerShowTeam--;
    }

    return (m_timerShowTeam > 0);
}

void Display::displayInfoCard(vector<Character> &persos, int xmouse, int ymouse)
{

    for(int i = 0 ; i < persos.size() ; i++)
    {
        if(xmouse/64 == persos[i].getCoord().x/64 && ymouse/64 == persos[i].getCoord().y/64)
        {
            if(xmouse < 256 && ymouse < 128)
            {
                createInfoCard(persos[i], 1);
            } else {
                createInfoCard(persos[i], 0);
            }
        }
    }

}

void Display::createInfoCard(Character &perso, int pos) {
    if (perso.estVivant())
    {
        SDL_SetRenderDrawColor(m_renderer, 160, 82, 45, 255);
        SDL_Rect rectCard;
        rectCard.x = pos * 18 * 64;
        rectCard.y = 0;
        rectCard.w = 256;
        rectCard.h = 128;
        SDL_RenderCopy(m_renderer, m_infoCard, NULL, &rectCard);

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

        int texteW, texteH;

        SDL_Color color = { 255, 255, 255, 255 };

        SDL_Texture* nom = charger_texte(perso.getName().c_str(), m_renderer, m_font8, color);
        SDL_QueryTexture(nom, NULL, NULL, &texteW, &texteH);

        SDL_Rect pos;
        pos.x = rectCard.x + 35;
        pos.y = 25;
        pos.w = texteW;
        pos.h = texteH;

        SDL_RenderCopy(m_renderer, nom, NULL, &pos);

        string ac = "ac ";
        ac += to_string(perso.getAc());
        SDL_Texture* acText = charger_texte(ac.c_str(), m_renderer, m_font8, color);
        SDL_QueryTexture(acText, NULL, NULL, &texteW, &texteH);

        pos.x = rectCard.x + rectCard.w - texteW - 40;
        pos.y = 25;
        pos.w = texteW;
        pos.h = texteH;

        SDL_RenderCopy(m_renderer, acText, NULL, &pos);

        string hp = to_string(perso.getHp());
        hp += "/";
        hp += to_string(perso.getMaxHp());
        SDL_Texture* hpStr = charger_texte(hp.c_str(), m_renderer, m_font8, color);
        SDL_QueryTexture(hpStr, NULL, NULL, &texteW, &texteH);

        
        pos.x = rectCard.x + 20;
        pos.y = 55;
        pos.w = texteW;
        pos.h = texteH;

        SDL_RenderCopy(m_renderer, hpStr, NULL, &pos);

        pos.x = rectCard.x + 90;
        pos.y = 55;
        pos.w = (rectCard.x + rectCard.w - 20) - pos.x;
        pos.h = pos.h;
        
        SDL_RenderCopy(m_renderer, m_underBar, NULL, &pos);

        pos.x = pos.x+2;
        pos.y = pos.y;
        pos.w = (pos.w * perso.getHp() / perso.getMaxHp())-4;
        pos.h = pos.h;

        SDL_RenderCopy(m_renderer, m_bar, NULL, &pos);
        
        SDL_QueryTexture(m_mana, NULL, NULL, &texteW, &texteH);

        pos.y = 72;
        pos.w = texteW;
        pos.h = texteH;

        for (unsigned i = 0; i < 5; i++)
        {
            pos.x = 47+36*i + rectCard.x;
            if (i+1 <= perso.getMana())
            {
                SDL_RenderCopy(m_renderer, m_mana, NULL, &pos);
            }
            else 
            {
                SDL_RenderCopy(m_renderer, m_blackMana, NULL, &pos);
            }
        }

        SDL_Rect iconRect;
        iconRect.y = 0;
        iconRect.w = 16;
        iconRect.h = 16;

        pos.x = rectCard.x + 40;
        pos.y = rectCard.h;
        pos.w = 32;
        pos.h = 32;

        if (perso.getTerrain() != Grass)
        {
            switch (perso.getTerrain())
            {
            case Forest:
                iconRect.x = 0;
                break;
            case Bastion:
                iconRect.x = 16;
                break;
            case Ruin:
                iconRect.x = 2 * 16;
                break;
            case River:
                iconRect.x = 3 * 16;
                break;
            default:
                break;
            }

            SDL_RenderCopy(m_renderer, m_statusMarker, &iconRect, &pos);
        }

        bool* stats = perso.getStatus();

        iconRect.y = 16;

        for (unsigned i = 0; i < 3; i++)
        {
            if (stats[i])
            {
                pos.x = pos.x + 5 + pos.w;
                iconRect.x = i*16;
                SDL_RenderCopy(m_renderer, m_statusMarker, &iconRect, &pos);
            }
        }

        iconRect.y = 32;

        for (unsigned i = 0; i < 3; i++)
        {
            if (stats[i+3])
            {
                pos.x = pos.x + 5 + pos.w;
                iconRect.x = i * 16;
                SDL_RenderCopy(m_renderer, m_statusMarker, &iconRect, &pos);
            }
        }

        delete[] stats;

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

        SDL_DestroyTexture(hpStr);
        SDL_DestroyTexture(nom);
    }
}

void Display::desallouer()
{
    TTF_CloseFont(m_font64);
    TTF_CloseFont(m_font16);
    TTF_CloseFont(m_font8);
    SDL_DestroyTexture(m_redMageTexture);
    SDL_DestroyTexture(m_blueMageTexture);
    SDL_DestroyTexture(m_endMageTexture);
    SDL_DestroyTexture(m_statusMarker);
    SDL_DestroyTexture(m_bar);
    SDL_DestroyTexture(m_underBar);
    SDL_DestroyTexture(m_infoCard);
    SDL_DestroyTexture(m_selectCase);
    SDL_DestroyTexture(m_spellSelect);
    SDL_DestroyTexture(m_spellNoSelect);
    SDL_DestroyTexture(m_d20);
    SDL_DestroyTexture(m_menu);
    SDL_DestroyTexture(m_mana);
    SDL_DestroyTexture(m_blackMana);
    SDL_DestroyTexture(m_background);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    SDL_Quit();
}
