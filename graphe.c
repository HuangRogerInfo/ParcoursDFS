/* Travail réalisé en binôme par Alain Barbier (11500443) et Huang Roger (11807555) - L3 Informatique */

#include "graphe.h"

#include <stdlib.h>	/* pour malloc() : fonction graph_init() */
#include <stdio.h>	/* pour printf() : fonction afficherMatriceAdj() */

graph_t * graph_init(int n) {
	graph_t * g = (graph_t *) malloc(sizeof(graph_t));

	if(g == NULL){
		printf("Erreur d'allocation du graphe!\n");
		abort(); /* sort du programme */
	}

	g->n = n;
	g->m = 0;
	g->matrice_adj = (int**) malloc(sizeof(int*) * n);

	if(g->matrice_adj == NULL){
		printf("Erreur d'allocation de la matrice d'adjacence!\n");
		abort(); /* sort du programme */
	}

	for(int i = 0; i < n; i++){
		g->matrice_adj[i] = (int*) malloc(sizeof(int) * n);

		if(g->matrice_adj[i] == NULL){
			printf("Erreur d'allocation des lignes de la matrice d'adjacence!\n");
			abort(); /* sort du programme */
		}
	}
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			g->matrice_adj[i][j] = 0;
		}
	}

	return g;
}

void freeGraph(graph_t * g) {
	for(int i = 0; i < g->n; i++){
		free(g->matrice_adj[i]);
	}

	free(g->matrice_adj);
	free(g);
}

void ajouterArete(graph_t * g,int s1,int s2) {
	if(s1 >= g->n || s2 >= g->n || s1 < 0 || s2 < 0){
		printf("Erreur : impossible d'ajouter une arete sur un sommet qui n'existe pas\n");
		return;
	}
	g->matrice_adj[s1][s2] = g->matrice_adj[s2][s1] = 1;
	g->m++;
}

void afficherMatriceAdj(graph_t * g) {
	printf("**** graphe de test:\n");
	printf("n = %d, m = %d\n",g->n,g->m);
	for(int i = 0; i < g->n; i++){
		for(int j = 0; j < g->n; j++){
			printf("%d  ",g->matrice_adj[i][j]);
		}
		printf("\n");
	}
}