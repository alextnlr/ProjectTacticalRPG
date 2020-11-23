#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"
#include "fonctions_SDL.h"

using namespace std;

Personnage::Personnage(int pv, Spell spell, char* nom, SDL_Texture* texture)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spell = spell;
    m_ptsMagie = 100;
    m_nom = nom;
    m_pos[0] = 0;
    m_pos[1] = 0;
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_select = false;
    m_frameIdle = 0;
    m_texture = texture;
    m_state = 0;
    m_mouvement = false;
    m_wait = false;
}

Personnage::Personnage(int pv, Spell spell, char* nom, int x, int y, SDL_Texture* texture)
{
    m_vieMax = pv;
    m_vie = m_vieMax;
    m_spell = spell;
    m_ptsMagie = 100;
    m_nom = nom;
    m_pos[0] = x;
    m_pos[1] = y;
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_select = false;
    m_frameIdle = 0;
    m_texture = texture;
    m_state = 0;
    m_mouvement = false;
    m_wait = false;
}

Personnage::Personnage(Personnage const& copie)
{
    m_vie = copie.m_vie;
    m_vieMax = copie.m_vieMax;
    m_spell = copie.m_spell;
    m_ptsMagie = copie.m_ptsMagie;
    m_nom = copie.m_nom;
    m_pos[0] = copie.m_pos[0];
    m_pos[1] = copie.m_pos[1];
    m_agro = false;
    m_cible[0] = 0;
    m_cible[1] = 0;
    m_hurt = false;
    m_dgtAnim = 0;
    m_select = false;
    m_frameIdle = 0;
    m_texture = copie.m_texture;
    m_state = 0;
    m_mouvement = false;
    m_wait = false;
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

bool Personnage::attaquer(Personnage &cible)
{
    if(m_cible[0] == cible.m_pos[0] && m_cible[1] == cible.m_pos[1])
    {
        cible.recevoirDegats(m_spell.getDegats());
        return true;
    } else {
        return false;
    }
}

void Personnage::afficherDegats(SDL_Renderer* renderer, TTF_Font* fontText)
{
    if(getHurt() && estVivant())
    {
        SDL_Color color = {0,0,0,0};
        char dgt[3];
        sprintf(dgt,"%i",m_hurt);
        SDL_Texture* text = charger_texte(dgt, renderer, fontText, color);
        int texteW, texteH;
        SDL_QueryTexture(text, NULL, NULL, &texteW, &texteH);

        SDL_Rect texte_pos;
        texte_pos.x = m_pos[0]*64+16;
        texte_pos.y = m_pos[1]*64-m_dgtAnim;
        texte_pos.w = texteW;
        texte_pos.h = texteH;
        SDL_RenderCopy(renderer, text, NULL, &texte_pos);

        if(m_dgtAnim > 20)
        {
            nonHurt();
            m_dgtAnim = 0;
        } else {
            m_dgtAnim++;
        }
    }
}

void Personnage::deplacer(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void Personnage::walk(int direction, int** mapTerrain, int lig, int col)
{
    switch(direction)
    {
    case 0:
        m_pos[1]--;
        if(m_pos[1] < 0 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[1]++;
        }
        break;
    case 1:
        m_pos[1]++;
        if(m_pos[1] > lig-1 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[1]--;
        }
        break;
    case 2:
        m_pos[0]++;
        if(m_pos[0] > col-1 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[0]--;
        }
        break;
    case 3:
        m_pos[0]--;
        if(m_pos[0] < 0 || mapTerrain[m_pos[1]][m_pos[0]] != 0)
        {
            m_pos[0]++;
        }
        break;
    }
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

void Personnage::afficherEtat() const
{
    cout << m_nom << " PV = " << m_vie << endl;
}

void Personnage::afficherSelect(SDL_Renderer* renderer) const
{
    SDL_Rect rect;
    rect.x = m_cible[0]*64;
    rect.y = m_cible[1]*64;
    rect.w = 64;
    rect.h = 64;

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

bool Personnage::getAgro() const
{
    return m_agro;
}

bool Personnage::getHurt() const
{
    return m_hurt;
}

void Personnage::nonHurt()
{
    m_hurt = false;
}

void Personnage::switchMode()
{
    if(m_agro)
    {
        m_agro = false;
    } else {
        m_agro = true;
    }
}

void Personnage::afficherRectSel(SDL_Renderer* renderer)
{
    SDL_Rect rect = getCoord();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void Personnage::afficherPersoBarre(SDL_Renderer* renderer, bool phy_frame)
{
    SDL_Rect rect = getCoord();
    if(estVivant())
    {
        SDL_Rect Frame;
        int trueFrame = m_frameIdle/3;
        Frame.x = 31*trueFrame;
        Frame.y = 31*m_state;
        Frame.w = 31;
        Frame.h = 31;

        SDL_RenderCopy(renderer, m_texture, &Frame, &rect);

        if(phy_frame)
        {
            m_frameIdle ++;
            if(m_frameIdle >= 4*3)
            {
                m_frameIdle = 0;
            }
        }
    }
}

void Personnage::setState(int state)
{
    m_state = state;
}

void Personnage::afficherInfos(SDL_Renderer* renderer, TTF_Font* font, bool pos)
{
    if(estVivant())
    {
        SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
        SDL_Rect rect;
        rect.x = pos*18*64;
        rect.y = 0;
        rect.w = 256;
        rect.h = 128;
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Color color = {0,0,0,0};
        char hp[10];
        sprintf(hp,"%i / %i",m_vie, m_vieMax);
        SDL_Texture* hpStr = charger_texte(hp, renderer, font, color);
        int texteW, texteH;
        SDL_QueryTexture(hpStr, NULL, NULL, &texteW, &texteH);

        SDL_Rect texte_pos;
        texte_pos.x = rect.x+(rect.w-texteW)/2;
        texte_pos.y = 60;
        texte_pos.w = texteW;
        texte_pos.h = texteH;

        SDL_RenderCopy(renderer, hpStr, NULL, &texte_pos);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect UnderBar;
        UnderBar.w = rect.w*2/3;
        UnderBar.h = 20;
        UnderBar.x = rect.x+(rect.w-UnderBar.w)/2;
        UnderBar.y = 90;
        SDL_RenderFillRect(renderer, &UnderBar);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &UnderBar);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect Bar;
        Bar.x = UnderBar.x;
        Bar.y = UnderBar.y;
        Bar.w = UnderBar.w*m_vie/m_vieMax;
        Bar.h = UnderBar.h;
        SDL_RenderFillRect(renderer, &Bar);

        SDL_Texture* nom = charger_texte(m_nom, renderer, font, color);
        SDL_QueryTexture(nom, NULL, NULL, &texteW, &texteH);

        SDL_Rect nom_pos;
        nom_pos.x = rect.x+(rect.w-texteW)/2;
        nom_pos.y = 25;
        nom_pos.w = texteW;
        nom_pos.h = texteH;

        SDL_RenderCopy(renderer, nom, NULL, &nom_pos);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    }
}

void Personnage::desallouer()
{
    SDL_DestroyTexture(m_texture);
}
