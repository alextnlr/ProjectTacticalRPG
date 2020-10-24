#ifndef FONCTIONS_FICHIER_H_INCLUDED
#define FONCTIONS_FICHIER_H_INCLUDED

char** allouer_tab_2D(int n, int m);
void desallouer_tab_2D(char** tab, int m);
void afficher_tab_2D(char** tab, int n, int m);
void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);

char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);

#endif // FONCTIONS_FICHIER_H_INCLUDED
