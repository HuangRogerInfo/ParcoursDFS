/* Travail réalisé en binôme par Alain Barbier (11500443) et Huang Roger (11807555) - L3 Informatique */
/*
	Pour compiler > gcc -Wall liste.c parcours.c graphe.c main.c -o prog.exe
*/

#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"
#include "parcours.h"
#include "liste.h"

/* *************************************** Fonctions annexes sur les listes ************************************* */

/* Renvoie une liste contenant la différence de deux listes (l1 privé de l2) */
liste * difference(liste * l1, liste * l2) {
	liste * resultat = liste_construire(liste_get_n(l1));
	int * n = liste_get_debut(l1);

	for(int i = 0; i < liste_get_taille(l1); i++){
		if(!liste_contient_element(l2,*n)){
			liste_ajouter_fin(resultat,*n);
		}
		n = liste_get_suivant(l1,n);
	}
	return resultat;
}

/* Renvoie l'élément minimum d'une liste */
int minimum_liste(liste * l) {
	for(int i = 0; i < liste_get_n(l); i++){
		if(liste_contient_element(l,i)){
			return i;
		}
	}
	return -1; /* Liste vide, aucun élément à renvoyer (on renvoie -1) */
}

/* Renvoie 1 si L1 est inclus dans L2, 0 sinon */
int inclusion(liste *l1, liste *l2) {
	int * n = liste_get_debut(l1);
	for(int i = 0; i < liste_get_taille(l1); i++){
		if(!liste_contient_element(l2,*n)){
			return 0; 
		}
		n = liste_get_suivant(l1,n);
	}
	return 1;
}

/* Renvoie une liste contenant l'ensemble des voisins d'un sommet s dans un graphe g */
liste * voisins(graph_t * g, int s) {
	liste * resultat = liste_construire(g->n);

	for(int i = 0; i < g->n ; i++){
		if(g->matrice_adj[s][i] >= 1){
			liste_ajouter_fin(resultat,i);
		}
	}

	return resultat;
}

/* *************************************** Algorithme Itératif ************************************* */

parcours_t * DFS_Iteratif(graph_t * g, int r) {

	parcours_t * p = parcours_init(g->n); /* On initialise un nouveau parcours */

	liste * M = liste_construire(g->n); /* La liste d'exploration M (l'ensemble des sommets visités) */
	liste * N = liste_construire(g->n); /* La liste de fin d'exploration N (l'ensemble des sommets où s'achève l'exploration) */
	liste * L = liste_construire(g->n); /* La pile de parcours L */

	int premier = 0;
	int v, w;

	while(!liste_est_pleine(M)){ /* Tant qu'il reste des sommets non-explorés du graphe (V\M != VIDE --> M != PLEIN) */
		if(premier != 0){
			for(int i = 0; i < g->n; i++){
				if(!liste_contient_element(M,i)){
					r = i; /* Choix du plus petit sommet non-exploré du graphe */
					break;
				}
			}
		}
		premier = 1; /* Ce n'est plus la première racine */
		p->pere[r] = -1; /* Ce sommet n'a pas de père */

		liste_ajouter_fin(L,r);
		liste_ajouter_fin(M,r);

		while(!liste_est_vide(L)){ /* Tant que la pile L n'est pas vide */

			v = *liste_get_fin(L); /* Le sommet de la pile L est dans "v" */
			liste * voisins_v = voisins(g,v); /* On crée l'ensemble des voisins du sommet v */
		
			if(inclusion(voisins_v,M) == 1){ /* Si il n'y a plus aucun voisin de v à explorer alors : */
				liste_supprimer_fin(L,&v); 
				liste_ajouter_fin(N,v);
			}
			else{ /* Sinon il reste des voisins non explorés */
				liste * voisins_v_privee_M = difference(voisins_v,M); /* On crée l'ensemble des voisins de v qui n'ont pas déjà été explorés */
				w = minimum_liste(voisins_v_privee_M); /* On choisit le plus petit voisin non-exploré est dans "w" */
				p->pere[w] = v; /* le père de w vaut v */

				p->aretes[p->nb_Aretes].sommet1 = v; /* On ajoute l'arete (v,w) dans le parcours */
				p->aretes[p->nb_Aretes].sommet2 = w;
				p->nb_Aretes++;

				liste_ajouter_fin(L,w); 
				liste_ajouter_fin(M,w);
				liste_detruire(&voisins_v_privee_M);
			}
			liste_detruire(&voisins_v);
		}
	}

	for(int j = 0; j < g->n; j++){
		p->prefixe[j] = M->pos[j] + 1;
		p->suffixe[j] = N->pos[j] + 1;
	}

	liste_detruire(&M);
	liste_detruire(&N);
	liste_detruire(&L);

	return p;
}

/* *************************************** Algorithme Récursif ************************************* */

void explorer(graph_t * g, int s, liste * M, liste * N, parcours_t * p) {

	liste_ajouter_fin(M,s);

	for(int i = 0; i < g->n ; i++){ /* On visite l'ensemble des voisins non-explorés de s */
		if(!liste_contient_element(M,i) && g->matrice_adj[s][i] >= 1){
			p->pere[i] = s; /* le père de i vaut s */

			p->aretes[p->nb_Aretes].sommet1 = s; /* On ajoute l'arete (s,i) dans le parcours */
			p->aretes[p->nb_Aretes].sommet2 = i;
			p->nb_Aretes++;

			explorer(g,i,M,N,p); /* Appel récursif, on explore le voisin i de s */
		}
	}

	liste_ajouter_fin(N,s); /* Si il n'y a plus de voisins à explorer on ajoute s à la liste de fin d'exploration N */
}

parcours_t * DFS_Recursif(graph_t * g, int r) {

	parcours_t * p = parcours_init(g->n); /* On initialise un nouveau parcours */

	liste * M = liste_construire(g->n); /* La liste d'exploration M (l'ensemble des sommets visités) */
	liste * N = liste_construire(g->n); /* La liste de fin d'exploration N (l'ensemble des sommets où s'achève l'exploration) */

	int premier = 0;

	while(!liste_est_pleine(M)){ /* Tant qu'il reste des sommets non-explorés du graphe (V\M != VIDE --> M != PLEIN) */
		if(premier != 0){
			for(int i = 0; i < g->n; i++){
				if(!liste_contient_element(M,i)){
					r = i; /* Choix du plus petit sommet non-exploré du graphe */
					break;
				}
			}
		}
		premier = 1; /* Ce n'est plus la première racine */
		p->pere[r] = -1; /* Ce sommet n'a pas de père */

		explorer(g,r,M,N,p); /* On explore le sommet r (on rappelle la fonction pour chaque composante connexe) */
	}

	for(int j = 0; j < g->n; j++){
		p->prefixe[j] = M->pos[j] + 1;
		p->suffixe[j] = N->pos[j] + 1;
	}

	liste_detruire(&M);
	liste_detruire(&N);

	return p;
}

int main() {

	graph_t * graph1 = graph_init(8);

	ajouterArete(graph1,0,1);
	ajouterArete(graph1,0,2);
	ajouterArete(graph1,1,3);
	ajouterArete(graph1,1,4);
	ajouterArete(graph1,2,3);
	ajouterArete(graph1,3,4);
	ajouterArete(graph1,4,5);
	ajouterArete(graph1,4,6);
	ajouterArete(graph1,5,6);
	ajouterArete(graph1,6,7);

	afficherMatriceAdj(graph1);

	parcours_t * parcours_I = DFS_Iteratif(graph1,0);
	parcours_t * parcours_R = DFS_Recursif(graph1,0);

	printf("*** donnees obtenues pour le parcours (version iterative):\n");
	afficherParcours(parcours_I);

	printf("*** donnees obtenues pour le parcours (version recursive):\n");
	afficherParcours(parcours_R);

	ecrire_dot(parcours_I,"Parcours_iteratif.dot");
	ecrire_dot(parcours_R,"Parcours_recursif.dot");

	freeParcours(parcours_I);
	freeParcours(parcours_R);
	freeGraph(graph1);
	return 0;
}