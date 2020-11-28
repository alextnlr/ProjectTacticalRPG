#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "spell.h"

class Personnage
{
public:
    Personnage(int pv, Spell spell, char* nom);
    Personnage(int pv, Spell spell, char* nom, int x, int y);
    Personnage(Personnage const& copie);
    void recevoirDegats(int nbDegats);
    bool attaquer(Personnage &cible);
    SDL_Rect afficherDegats(int texteW, int texteH);
    void deplacer(int x, int y);
    void walk(int direction, int** mapTerrain, int lig, int col);
    void heal(int qteHeal);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    void afficherEtat() const;
    void afficherSelect(SDL_Renderer* renderer) const;
    bool getAgro() const;
    int getHurt() const;
    int getState() const;
    int getHp() const;
    int getMaxHp() const;
    char* getName() const;
    int getWait() const;
    void switchMode();
    SDL_Rect afficherPersoBarre(bool phy_frame);
    void setState(int state);
    void setWait(int temps);
    void decreaseWait();
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
    int m_frameIdle;
    int m_state;
    bool m_mouvement;
    int m_wait;
};

#endif // PERSONNAGE_H_INCLUDED
