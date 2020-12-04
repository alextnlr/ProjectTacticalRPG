#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED

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
    void displayTerrain(const MaptabP *map);
    void displayCharacters(vector<Personnage> &persos);
    bool displayDamages(vector<Personnage>& persos, int roll);
    bool displayRoll(int& roll);
    void displayMenu(vector<Personnage> &persos);
    void displaySpellRange(vector<Personnage> &persos, const MaptabP *map);
    bool displayTeam(int team, int roll);
    void displayInfoCard(vector<Personnage> &persos, int xmouse, int ymouse);
    void createInfoCard(Personnage &perso, int pos);
    void desallouer();
private:
    int m_widthWindow;
    int m_heightWindow;
    int m_currentTeam;
    int m_timerShowTeam;
    int m_timerShowRoll;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font64;
    TTF_Font* m_font16;
    TTF_Font* m_font8;
    SDL_Texture* m_infoCard;
    SDL_Texture* m_menu;
    SDL_Texture* m_underBar;
    SDL_Texture* m_bar;
    SDL_Texture* m_mana;
    SDL_Texture* m_blackMana;
    SDL_Texture* m_d20;
    SDL_Texture* m_blueMageTexture;
    SDL_Texture* m_redMageTexture;
    SDL_Texture* m_endMageTexture;
    SDL_Texture* m_background;
    bool m_physicalFrame;
    int m_frameNum;
    int const m_TICK_INTERVALLE = 60;
    SDL_Rect m_rectPos;
    vector<SDL_Rect> m_rectTerrain;
    bool m_found;
    int m_select;
    int m_stockRoll;
};

#endif // AFFICHAGE_H_INCLUDED
