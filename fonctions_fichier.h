#ifndef FONCTIONS_FICHIER_H_INCLUDED
#define FONCTIONS_FICHIER_H_INCLUDED

char** allouer_tab_2D(int n, int m);
void desallouer_tab_2D(char** tab, int m);
void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);

int** transfoCharToInt(char** tabChar, int lig, int col);
int** allouer_tab_2DInt(int n, int m);
void desallouer_tab_2D(int** tab, int n);

#endif // FONCTIONS_FICHIER_H_INCLUDED
