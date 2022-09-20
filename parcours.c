/* Travail réalisé en binôme par Alain Barbier (11500443) et Huang Roger (11807555) - L3 Informatique */

#include "parcours.h"

#include <stdlib.h>	/* pour malloc() : fonction parcours_init() */
#include <stdio.h>	/* pour printf() : fonction afficherParcours() */

parcours_t * parcours_init(int n) {
	parcours_t * p = (parcours_t *) malloc(sizeof(parcours_t));

	if(p == NULL){
		printf("Erreur d'allocation du parcours!\n");
		abort(); /* sort du programme */
	}

	p->prefixe = (int*) malloc(sizeof(int) * n);

	if(p->prefixe == NULL){
		printf("Erreur d'allocation du tableau d'entiers prefixe!\n");
		abort(); /* sort du programme */
	}

	p->suffixe = (int*) malloc(sizeof(int) * n);

	if(p->suffixe == NULL){
		printf("Erreur d'allocation du tableau d'entiers suffixe!\n");
		abort(); /* sort du programme */
	}

	p->pere = (int*) malloc(sizeof(int) * n);

	if(p->pere== NULL){
		printf("Erreur d'allocation du tableau d'entiers pere!\n");
		abort(); /* sort du programme */
	}

	p->aretes = (arete_t*) malloc(sizeof(arete_t) * n * 2);

	if(p->aretes == NULL){
		printf("Erreur d'allocation du tableau d'arretes!\n");
		abort(); /* sort du programme */
	}

	p->nb_Aretes = 0;
	p->n = n;

	return p;
}

void freeParcours(parcours_t * p) {
	free(p->prefixe);
	free(p->suffixe);
	free(p->pere);
	free(p->aretes);
	free(p);
}

void afficherParcours(parcours_t * p) {
	printf("sommet  prefixe  suffixe  pere\n");
	for(int i = 0; i < p->n; i++){
		printf("%d\t%d\t %d\t  ", i, p->prefixe[i], p->suffixe[i]);
		if(p->pere[i] == -1){
			printf("- \n");
		}
		else{
			printf("%d \n", p->pere[i]);
		}
	}
	printf("*** Ensemble des aretes :\n { ");
	for(int j = 0; j < p->nb_Aretes; j++){
		printf("(%d,%d), ", p->aretes[j].sommet1, p->aretes[j].sommet2);
	}
	printf("}\n");
}

int ecrire_dot(parcours_t * p, const char * nom_fichier) {
	FILE * f = NULL;
	f = fopen(nom_fichier,"w");

	if(f == NULL) {
		perror("fopen"); /* écrit la dernière erreur rencontrée */
		return 1;
	}

	fprintf(f, "digraph {\n");

	for(int i = 0; i < p->n; i++){
		fprintf(f,"\t%d;\n", i);
	}

	fprintf(f,"\n");

	for(int j = 0; j < p->n; j++){
		if(p->pere[j] != -1){
			fprintf(f,"\t%d -> %d;\n",p->pere[j],j);
		}
	}

	fprintf(f,"}");
	fclose(f);

	return 0;
}