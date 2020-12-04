#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int poids(int x_dep, int y_dep, int x_arr, int y_arr){
	int res;
	res = sqrt((x_arr - x_dep)*(x_arr - x_dep) + (y_arr - y_dep)*(y_arr - y_dep));
	return res;
}

void plus_court_chemin(int chemin, int x_dep, int y_dep, int x_arr, int y_arr){
	// on fait une liste vide pour le chemin
	chemin = ConsVide();
	// tant qu'on est pas arrivé au point d'arrivé on continue
	while(x_dep != x_arr && y_dep != y_arr){
		// exploration a droite
		while(!obstacle){
			int x_d = x_dep;
			int y_d = y_dep;
			int chemin_d = ConsVide();
			chemin_d = Cons((x_d, y_d), ConsVide);
			x_d = x_d + 1;
			int poids_d = poids (x_d, y_d, x_dep, y_dep);
		}
		//exploration a gauche
		while(!obstacle){
			int x_g = x_dep;
			int y_g = y_dep;
			int chemin_g = ConsVide();
			chemin_g = Cons((x_g, y_g), chemin_g);
			x_g = x_g - 1;
			int poids_g = poids (x_g, y_g, x_dep, y_dep);
		}
		// exploration en haut
		while(!obstacle){
			int x_h = x_dep;
			int y_h = y_dep;
			int chemin_h = ConsVide();
			chemin_h = Cons((x_h, y_h), ConsVide);
			y_h = y_h + 1;
			int poids_h = poids (x_h, y_h, x_dep, y_dep);
		}
		// exploration en bas
		while(!obstacle){
			int x_b = x_dep;
			int y_b = y_dep;
			int chemin_b = ConsVide();
			chemin_b = Cons((x_b, y_b), ConsVide);
			y_b = y_b - 1;
			int poids_b = poids (x_b, y_b, x_dep, y_dep);
		}
		//comparaison des poids
		// si c'est le chemin droit le plus court
		if (poids_d < poids_g && poids_d < poids_h && poids_d < poids_b){
			chemin = Cons(chemin_d, chemin);
		}
		// si c'est le chemin gauche le plus court
		if(poids_g < poids_d && poids_g < poids_h && poids_g < poids_b){
			chemin = Cons(chemin_g, chemin);
		}
		// si c'est le chemin haut le plus court
		if (poids_h < poids_g && poids_h < poids_d && poids_h < poids_b){
			chemin = Cons(chemin_h, chemin);
		}
		// si c'est le chemin bas le plus court
		if(poids_b < poids_g && poids_b < poids_h && poids_b < poids_d){
			chemin = Cons(chemin_b, chemin);
		}
	}
}

