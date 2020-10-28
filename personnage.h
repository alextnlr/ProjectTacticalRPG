#ifndef PERSONNAGE_H_INCLUDED
#define PERSONNAGE_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Personnage
{
public:
    Personnage(SDL_Texture* texture);
    Personnage(int pv, int dgt, char* nom, SDL_Texture* texture);
    Personnage(int pv, int dgt, char* nom, int x, int y, SDL_Texture* texture);
    Personnage(Personnage const& copie);
    void recevoirDegats(int nbDegats);
    bool attaquer(Personnage &cible);
    void afficherDegats(SDL_Renderer* renderer, TTF_Font* fontText);
    void deplacer(int x, int y);
    void walk(int direction);
    void heal(int qteHeal);
    void select(int direction);
    SDL_Rect getCoord() const;
    bool estVivant() const;
    void afficherEtat() const;
    void afficherSelect(SDL_Renderer* renderer) const;
    bool getAgro() const;
    int getDegats() const;
    bool getHurt() const;
    void nonHurt();
    void switchMode();
    bool getSelect() const;
    void selectPerso();
    void deselectPerso();
    void afficherPersoBarre(SDL_Renderer* renderer, bool phy_frame);
    void setState(int state);
    void afficherInfos(SDL_Renderer* renderer, TTF_Font* font, bool gauche);
    ~Personnage();

private:
    int m_vie;
    int m_vieMax;
    int m_ptsMagie;
    char* m_nom;
    int m_pos[2];
    int m_degats;
    bool m_agro;
    int m_hurt;
    int m_cible[2];
    int m_dgtAnim;
    bool m_select;
    SDL_Texture* m_texture;
    int m_frameIdle;
    int m_state;
};

#endif // PERSONNAGE_H_INCLUDED
