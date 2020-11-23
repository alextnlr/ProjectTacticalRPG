#include <iostream>
#include "fonctions_fichier.h"

using namespace std;

char** allouer_tab_2D(int n, int m) {
    char** tableau = new char*[n];
    if(tableau == nullptr) {
        cout << "Echec de l'allocation" << endl;
        exit(EXIT_FAILURE);
    }

    for (unsigned i = 0 ; i < n ; i++) {
        tableau[i] = new char[m];
        if(tableau[i]==nullptr) {
            cout << "Echec de l'allocation\n" << endl;
            exit(EXIT_FAILURE);
        }
    }

    for(unsigned i = 0 ; i < n ; i++) {
        for(unsigned j = 0 ; j < m ; j++) {
            tableau[i][j] = ' ';
        }
    }

    return tableau;
}

int** allouer_tab_2DInt(int n, int m)
{
    int** tableau = new int*[n];
    if(tableau == nullptr) {
        cout << "Echec de l'allocation" << endl;
        exit(EXIT_FAILURE);
    }

    for (unsigned i = 0 ; i < n ; i++) {
        tableau[i] = new int[m];
        if(tableau[i]==nullptr) {
            cout << "Echec de l'allocation\n" << endl;
            exit(EXIT_FAILURE);
        }
    }

    for(unsigned i = 0 ; i < n ; i++) {
        for(unsigned j = 0 ; j < m ; j++) {
            tableau[i][j] = ' ';
        }
    }

    return tableau;
}

void desallouer_tab_2D(char** tab, int n) {

    for(unsigned i = 0 ; i < n ; i++) {
        delete[] tab[i];
    }

    delete [] tab;
}

void desallouer_tab_2D(int** tab, int n) {

    for(unsigned i = 0 ; i < n ; i++) {
        delete[] tab[i];
    }

    delete [] tab;
}

void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol) {
    FILE* fichier = nullptr;
    fichier = fopen(nomFichier,"r");
    if(fichier == nullptr) {
        cout << "Echec de l'ouverture" << endl;
        exit(EXIT_FAILURE);
    }

    char chaine[40] = "";
    int col = 0;
    int lig = 0;

    while(fgets(chaine, 40, fichier) != NULL) {
        lig++;
        for(unsigned i = 0 ; i < 40 ; i++) {
            if(isgraph(chaine[i])) {col++;}
        }
        if(col > *nbCol) { *nbCol = col; }
        col = 0;
        for(unsigned j = 0 ; j<40 ; j++) {
            chaine[j] = ' ';
        }
    }
    *nbLig = lig;

    fclose(fichier);
}

char** lire_fichier(const char* nomFichier) {

    FILE* fichier = nullptr;
    fichier = fopen(nomFichier,"r");
    if(fichier == nullptr) {
        cout << "Echec de l'ouverture" << endl;
        exit(EXIT_FAILURE);
    }

    int n=0, m=0;
    taille_fichier(nomFichier, &n, &m);
    char** tab = allouer_tab_2D(n,m);
    char chaine[40] = " ";

    for(unsigned i = 0 ; i < n ; i++) {
        fgets(chaine, 40, fichier);
        for(unsigned j = 0 ; j < m ; j++) {
            if(isgraph(chaine[j])) {tab[i][j] = chaine[j];}
        }
        for(unsigned j = 0 ; j<40 ; j++) {
            chaine[j] = ' ';
        }
    }

    return tab;
}

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m) {

    FILE* fichier = nullptr;
    fichier = fopen(nomFichier,"w");
    if(fichier == nullptr) {
        cout << "Echec de l'ouverture" << endl;
        exit(EXIT_FAILURE);
    }

    for(unsigned i = 0 ; i < n ; i++) {
        for(unsigned j = 0 ; j < m ; j++) {
            fputc(tab[i][j],fichier);
        }
        fputc('\n',fichier);
    }

}

int** transfoCharToInt(char** tabChar, int lig, int col)
{
    int** tabInt = allouer_tab_2DInt(lig, col);
    for(int i = 0 ; i < lig ; i++) {
        for(int j = 0 ; j < col ; j++) {
            tabInt[i][j] = tabChar[i][j] - '0';
        }
    }
    return tabInt;
}
