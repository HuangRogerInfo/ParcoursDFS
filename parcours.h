/* Travail réalisé en binôme par Alain Barbier (11500443) et Huang Roger (11807555) - L3 Informatique */

#ifndef PARCOURS_H
#define PARCOURS_H

typedef struct arete_s {
	int sommet1;
	int sommet2;
} arete_t;

typedef struct parcours_s {
	int * prefixe;
	int * suffixe;
	int * pere;
	arete_t * aretes; /* l'ensemble des aretes du parcours */
	int nb_Aretes;
	int n; /* taille des tableaux : prefixe, suffixe et pere */
} parcours_t;

/* Initialise un parcours de taille n */
parcours_t * parcours_init(int n);

/* Libère la mémoire d'un parcours p */
void freeParcours(parcours_t * p);

/* Affiche un parcours p (prefixe, suffixe et père) */
void afficherParcours(parcours_t * p);

/* Crée un fichier .dot à partir d'un parcours p */
int ecrire_dot(parcours_t * p, const char * nom_fichier);

#endif