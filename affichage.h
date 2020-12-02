#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "personnage.h"

using namespace std;

class Affichage
{
public:
    Affichage();
    void setRectTerrain(int nbColonnes, int nbLignes);
    void clearRenderer() const;
    void displayRenderer() const;
    void setFrame();
    void displayTerrain(int** mapTerrain, int lig, int col);
    void displayCharacters(vector<Personnage> &allies, vector<Personnage> &ennemies);
    void displaySpellRange(vector<Personnage> &allies, int** map, int lig, int col);
    void displayInfoCard(vector<Personnage> &allies, vector<Personnage> &ennemies, int xmouse, int ymouse);
    void createInfoCard(Personnage &perso, int pos);
    SDL_Renderer* getRenderer() const;
    TTF_Font* getFontDmg() const;
    bool getPhysicalFrame() const;
    void desallouer();
private:
    int m_widthWindow;
    int m_heightWindow;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font16;
    TTF_Font* m_font8;
    SDL_Texture* m_infoCard;
    SDL_Texture* m_underBar;
    SDL_Texture* m_bar;
    SDL_Texture* m_mana;
    SDL_Texture* m_blackMana;
    SDL_Texture* m_blueMageTexture;
    SDL_Texture* m_redMageTexture;
    SDL_Texture* m_background;
    bool m_physicalFrame;
    int m_frameNum;
    int const m_TICK_INTERVALLE = 60;
    SDL_Rect m_rectPos;
    vector<SDL_Rect> m_rectTerrain;
    bool m_found;
    int m_select;
};

#endif // AFFICHAGE_H_INCLUDED
