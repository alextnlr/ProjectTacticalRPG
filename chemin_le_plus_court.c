#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// calcul de la distance entre la case où on va et le point d'arrivée
int poids(int x_dep, int y_dep, int x_arr, int y_arr){
	int res;
	res = sqrt((x_arr - x_dep)*(x_arr - x_dep) + (y_arr - y_dep)*(y_arr - y_dep));
	return res;
}

// on instancie le tableau que avec des 0 pour ne pas retourner sur ses pas
int** tabeau(int n, int m){
	int** tab = malloc(n*sizeof(int));
	for(int i=0; i<n; i++){
		tab[i] = malloc(m*sizeof(int));
		for(int j=0; j<m; j++){
			tab[i][j] = 0;
		}
	}
}

void plus_court_chemin(int chemin, int x_dep, int y_dep, int x_arr, int y_arr, int** tab){
	// on fait une liste et on enregistre la position de départ
	chemin = cons((x_dep,y_dep), chemin);
	
	// Tant qu'on est pas arrivé on continue
	while(x_dep != x_arr && y_dep != y_arr){
		// si la case du tableau à droite est libre alors
		int poids_d = 1000;
		int x_d;
		int y_d;
		if(tab[x_dep+1][y_dep] = 0){
			x_d = x_dep+1;
			y_d = y_dep;
			poids_d = poids (x_d, y_d, x_arr, y_arr);
		}
		
		// Si la case du tableau à gauche est libre alors
		int poids_g = 1000;
		int x_g;
		int y_g;
		if(tab[x_dep-1][y_dep] = 0){
			x_g = x_dep-1;
			y_g = y_dep;
			poids_g = poids (x_g, y_g, x_arr, y_arr);
		} 
		
		// Si la case du tableau en haut est libre alors
		int poids_h = 1000;
		int x_h;
		int y_h;
		if(tab[x_dep][y_dep-1] = 0){
			x_h = x_dep;
			y_h = y_dep-1;
			poids_h = poids(x_h, y_h, x_arr, y_arr);
		}
		
		// Si la case de tableau en bas est libre alors
		int poids_b = 1000;
		int x_b;
		int y_b;
		if(tab[x_dep+1][y_dep] = 0){
			x_b = x_dep;
			y_b = y_dep+1;
			poids_b = poids(x_b, y_b, x_arr, y_arr);
		}
		
		// Si c'est le poids_d le plus léger alors c'est par là qu'il faut passer
		if(poids_d<poids_g && poids_d<poids_h && poids_d<poids_b){
			chemin = Cons((x_d, y_d), chemin);
			tab[x_dep][y_dep] = 1;
			x_dep = x_d;
			y_dep = y_d;
		} else {
			// le poids_d n'est pas le plus léger alors si c'est le poids_g
			if(poids_g<poids_h && poids_g<poids_b){
				chemin = Cons((x_g, y_g), chemin);
				tab[x_dep][y_dep] = 1;
				x_dep = x_g;
				y_dep = y_g;
			} else {
				// ni le poids_d ni le poids_g ne sont les plus léger alors si c'est le poids_h
				if(poids_h<poids_b){
					chemin = Cons((x_h, y_h), chemin);
					tab[x_dep][y_dep] = 1;
					x_dep = x_h;
					y_dep = y_h;
				} else {
					// le poids_b est le plus léger
					chemin = Cons((x_b, y_b), chemin);
					tab[x_dep][y_dep] = 1;
					x_dep = x_b;
					y_dep = y_b;
				}
			}
		}
	}
}
