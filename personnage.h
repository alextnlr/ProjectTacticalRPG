#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "spell.h"

class Personnage
{
public:
    Personnage(int pv, Spell spell, char* nom, SDL_Texture* texture);
    Personnage(int pv, Spell spell, char* nom, int x, int y, SDL_Texture* texture);
    Personnage(Personnage const& copie);
    void recevoirDegats(int nbDegats);
    bool attaquer(Personnage &cible);
    void afficherDegats(SDL_Renderer* renderer, TTF_Font* fontText);
    void deplacer(int x, int y);
    void walk(int direction, int** mapTerrain, int lig, int col);
    void heal(int qteHeal);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    void afficherEtat() const;
    void afficherSelect(SDL_Renderer* renderer) const;
    bool getAgro() const;
    bool getHurt() const;
    void nonHurt();
    void switchMode();
    void afficherPersoBarre(SDL_Renderer* renderer, bool phy_frame);
    void afficherRectSel(SDL_Renderer* renderer);
    void setState(int state);
    void afficherInfos(SDL_Renderer* renderer, TTF_Font* font, bool gauche);
    void desallouer();

private:
    int m_vie;
    int m_vieMax;
    int m_ptsMagie;
    char* m_nom;
    int m_pos[2];
    Spell m_spell;
    bool m_agro;
    int m_hurt;
    int m_cible[2];
    int m_dgtAnim;
    bool m_select;
    SDL_Texture* m_texture;
    int m_frameIdle;
    int m_state;
    bool m_mouvement;
    bool m_wait;
    int m_direction;
};

#endif // PERSONNAGE_H_INCLUDED
