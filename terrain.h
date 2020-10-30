#ifndef TERRAIN_H_INCLUDED
#define TERRAIN_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"

using namespace std;

class Terrain
{
public:
    Terrain(SDL_Renderer* renderer, vector<Personnage> allies);
    Terrain(SDL_Renderer* renderer, vector<Personnage> allies, char* nom_map, char* nom_case, int xcase, int ycase);
    void ajouterEnnemi(int pv, int dgt, char* nom, int posx, int posy);
    void setFrame();
    void afficherTerrain(SDL_Renderer* renderer);
    void afficherPersos(SDL_Renderer* renderer);
    void afficherDegats(SDL_Renderer* renderer, TTF_Font* fontText);
    void afficherAgro(SDL_Renderer* renderer);
    void afficherInfos(SDL_Renderer* renderer, TTF_Font* fontText, int xmouse, int ymouse);
    void switchMvtWait();
    void deplacerSelect(int dir);
    void switchMode();
    void select(int xmouse, int ymouse);
    void deselect();
    void desallouer();
private:
    int const m_TICK_INTERVALLE = 60;

    char** m_mapChar;
    int** m_map;
    int m_lig;
    int m_col;
    bool m_physical_frame;
    bool m_mouvement;
    int m_direction;
    bool m_wait;
    bool m_found;
    int m_frameNum;
    int m_select;
    SDL_Texture* m_case;
    vector<SDL_Rect> m_bloc;
    SDL_Rect m_blocPos;
    vector<Personnage> m_ennemis;
    vector<Personnage> m_allies;
    SDL_Texture* m_mageTextureEvil;
};

#endif // TERRAIN_H_INCLUDED
