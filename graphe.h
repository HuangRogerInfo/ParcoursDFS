/* Travail réalisé en binôme par Alain Barbier (11500443) et Huang Roger (11807555) - L3 Informatique */

#ifndef GRAPHE_H
#define GRAPHE_H

typedef struct graph_s {
	int n; /* nombre de sommets du graphe */
	int m; /* nombre d'aretes du graphe */
	int ** matrice_adj; /* matrice d'adjacence du graphe */
} graph_t;

/* Initialise un graphe de taille n */
graph_t * graph_init(int n);

/* Libère la mémoire d'un graphe g */
void freeGraph(graph_t * g);

/* Ajoute une arete (s1,s2) à un graphe g */
void ajouterArete(graph_t * g,int s1,int s2);

/* Afficher la matrice d'adjacence d'un graphe g */
void afficherMatriceAdj(graph_t * g);

#endif