#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Calcul de la distance entre la case où on va et le point d'arrivé
int poids(int x_dep, int y_dep, int x_arr, int y_arr){
	int res;
	res = sqrt((x_arr - x_dep)*(x_arr - x_dep) + (y_arr - y_dep)*(y_arr - y_dep));
	return res;
}

void plus_court_chemin(int chemin, int x_dep, int y_dep, int x_arr, int y_arr, int tab){
	// On fait une liste et on enregistre la position de départ
	chemin = Cons((x_dep, y_dep),chemin);
	
	// On instancie le tableau que avec des 0 pour ne pas retourner sur ses pas
	char** tableau(int n, int m){
		char** tab = malloc(n*sizeof(char));
		for(int i=0; i<n; i++){
			tab[i] = malloc(m*sizeof(char));
			for(int j=0; j<m; j++){
				tab[i][j] = 0;
			}
		}
	}
	
	// Tant qu'on est pas arrivé on continue
	while(x_dep != x_arr && y_dep != y_arr){
		//Si la case du tableau à droite est libre alors
		//if(tab[i][j]=0){
			int x_d = x_dep+1;
			int y_d = y_dep;
			int chemin_d = Cons((x_d,y_d), chemin);
			int poids_d = poids(x_d, y_d, x_arr, y_arr);
		//}
		
		// Si la case du tableau à gauche est libre alors
		//if(tab[i][j])=0){
			int x_g = x_dep-1;
			int y_g = y_dep;
			int chemin_g = Cons((x_g, y_g), chemin);
			int poids_g = poids(x_g, y_g, x_arr, y_arr);
		//}
		
		// Si la case du tableau en haut est libre alors
		//if(tab[i][j]=0){
			int x_h = x_dep;
			int y_h = y_dep+1;
			int chemin_h = Cons((x_h,y_h), chemin);
			int poids_h = poids(x_h, y_h, x_arr, y_arr);
		//}
		
		// Si la case du tableau en bas est libre alors
		//if(tab[i][j]=0){
			int x_b = x_dep;
			int y_b = y_dep-1;
			int chemin_b = Cons((x_b, y_b), chemin);
			int poids_b = poids(x_b, y_b, x_arr, y_arr);
		//}
		
		//Si c'est le poids_d le plus léger alors c'est par là qu'il faut passer
		if(poids_d<poids_g && poids_d<poids_h && poids_d<poids_b){
			chemin = Cons(chemin_d, chemin);
			x_dep = x_d;
			y_dep = y_d;
			//tab[][]=1
		} else {
			// le poids_d n'est pas le plus léger alors si c'est le poids g
			if(poids_g<poids_h && poids_g<poids_b){
				chemin = Cons(chemin_g, chemin);
				x_dep = x_g;
				y_dep= y_g;
				//tab[][]=1
			} else {
				// ni le poids_d ni le poids_g ne sont les plus léger alors si c'est le poids_h
				if(poids_h<poibs_b){
					chemin = Cons(chemin_h, chemin);
					x_dep = x_h;
					y_dep = y_h;
					//tab[][]=1
				} else{
					// Les poids_b est le plus léger
					chemin = Cons(chemin_b, chemin);
					x_dep = x_b;
					y_dep = y_b;
					//tab[][]=1
				}
			}
		}
	}
}
