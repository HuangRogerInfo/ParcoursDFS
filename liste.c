/*	liste : séquences d'entiers de Z_n étant donné un entier n >= 1 qui ne sont accessibles que par leur début ou leur fin :: fichier source
*/

#include "liste.h"

#include <stdlib.h>	/* pour malloc() : fonction liste_construire() */
#include <stdio.h>	/* pour printf() : fonction liste_afficher() */

#define LISTE_DEBUG 0	/* constante symbolique pour activer (LISTE_DEBUG != 0) ou désactiver (LISTE_DEBUG == 0) les traces */

/* ______________________________________ Indices 
*/

/* Indice suivant d'un indice ind sur Z_n : (ind +1) mod n */
#define LISTE_SUIV(n, ind) (ind +1 >= n ? (ind +1) % n : ind +1)

/* Indice précédent d'un indice ind sur Z_n : (ind -1) mod n */
#define LISTE_PREC(n, ind) (ind -1 < 0 ? n -(1 -ind) % n : ind -1)

/* ______________________________________ Destruction / Construction / Initialisation
*/

/* Cette fonction construit, initialise et renvoie une liste vide de capacité n >= 1 en cas de succès, elle renvoie NULL en cas d'échec (allocation mémoire ou n <= 0)
*/
liste* liste_construire(int n) {
	liste* l;		/* liste que l'on cherche à construire */
	int v;	/* variable de boucle */

	#if(LISTE_DEBUG)
	printf("%s(%d) IN\n", __func__, n);
	#endif

	/* __ vérification des paramètres */
	if(n <= 0)
		return NULL;

	/* __ allocation mémoire pour la liste */
	l = (liste*) malloc (sizeof(liste));
	if (l != NULL) {
		/* __ capacité de la liste */
		l->n = n;

		/* __ allocation mémoire pour les tableaux */

		/* l->tab */
		l->tab = (int*)malloc(l->n * sizeof(int));
		if(l->tab == NULL)
			liste_detruire(& l);
		else {
			/* l->pos */
			l->pos = (int*)malloc(l->n * sizeof(int));
			if(l->pos == NULL)
				liste_detruire(& l);	
			/* __ initialisation de l à la liste vide */
			else {
				/* curseur de début */
				l->d = -1;

				/* tableau des positions */
				for (v =0 ; v < l->n ; v ++)
					l->pos[v] = -1;
			}
		}
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: l = %p\n", __func__, (void*)l);
	#endif

	/* retour de la liste ainsi construite (... ou pas !) */
	return l;
}

/* Cette fonction construit et renvoie en cas de succès une copie d'une liste passée en argument ; en cas d'échec (allocation mémoire), elle renvoie NULL 
*/
liste* liste_copie(liste* l_source) {
	liste* l = NULL;		/* liste que l'on cherche à construire */
	int* i;	/* variable de boucle */

	#if(LISTE_DEBUG)
	printf("%s(%p) IN\n", __func__, (void*)l_source);
	#endif

	/* __ construction d'une lide vide */
	l = liste_construire(l_source->n);
	
	/* __ que l'on remplit par insertion systématique à la fin des éléments contenus dans l_source */
	for (i = liste_get_debut(l_source) ; i != NULL ; i = liste_get_suivant(l_source, i))
		liste_ajouter_fin(l, *i);

	#if(LISTE_DEBUG)
	printf("%s OUT: l = %p\n", __func__, (void*)l);
	#endif

	/* retour de la liste ainsi construite (... ou pas !) */
	return l;
}

/* Cette fonction vide une liste donnée
*/
void liste_vider(liste* l) {
	int v;

	while(! liste_est_vide(l))
		liste_supprimer_debut(l, &v);
}

/* Cette fonction libère la mémoire allouée pour une liste (et réinitialise à NULL le pointeur *l)
*/
void liste_detruire(liste** l) {
	#if(LISTE_DEBUG)
	printf("%s(%p) IN: *l = %p\n", __func__, (void*)l, (void*)(*l));
	#endif

	if(*l != NULL) {
		/* __ libération de la mémoire allouée pour les tableaux de la structure
			(les tableaux doivent être considérés dans l'ordre de leur construction) 
		*/
		if((*l)->tab != NULL) {
			free((*l)->tab);

			if((*l)->pos != NULL)
				free((*l)->pos);
		}

		/* __ libération de la mémoire allouée pour la structure elle-même */
		free(*l);

		/* __ gestion de la valeur NULL */
		*l = NULL;
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: *l = %p\n", __func__, (void*)(*l));
	#endif
}

/* ______________________________________ Insertion / Supression
*/

/* Cette fonction ajoute un élément donné en début de liste s'il ne s'y trouve pas déjà (auquel cas elle renvoie 0), elle renvoie -1 sinon
*/
int liste_ajouter_debut(liste* l, int elt)
{
	int statut = -1;		/* statut de réussite (statut == 0) ou d'échec (statut == -1) de la fonction */

	#if(LISTE_DEBUG)
	printf("%s(l = %p, elt = %d) IN : l->d = %d, l->f = %d\n", __func__, (void*)l, elt, l->d, l->f);
	#endif

	/* __ L'élément ne peut être ajouté que s'il ne figure pas déjà dans la liste */
	if (l->pos[elt] == -1) {
		statut = 0;

		/* __ Mise à jour les curseurs d et f
		*/

		/* liste vide : les cases d et f sont la case 0 */
		if(l->d == -1) {
			l->d = 0;
			l->f = 0;
		}
		/* liste non vide : décalage de d sur la case précédente */
		else
			l->d = LISTE_PREC(l->n, l->d);

		/* __ Insertion de l'élément en la nouvelle position d */
		l->tab[l->d] = elt;
		l->pos[elt] = l->d;
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: statut = %d, l->d = %d, l->f = %d\n", __func__, statut, l->d, l->f);
	#endif

	/* retour du statut de réussite ou d'échec (élément figurant déjà dans la liste) */
	return statut;
}

/* Cette fonction ajoute un élément donné en fin de liste s'il ne s'y trouve pas déjà (auquel cas elle renvoie 0), elle renvoie -1 sinon
*/
int liste_ajouter_fin(liste* l, int elt)
{
	int statut = -1;		/* statut de réussite (statut == 0) ou d'échec (statut == -1) de la fonction */

	#if(LISTE_DEBUG)
	printf("%s(l = %p, elt = %d) IN : l->d = %d, l->f = %d\n", __func__, (void*)l, elt, l->d, l->f);
	#endif

	/* __ L'élément ne peut être ajouté que s'il ne figure pas déjà dans la liste */
	if (l->pos[elt] == -1) {
		statut = 0;

		/* __ Mise à jour les curseurs d et f
		*/

		/* liste vide : les cases d et f sont la case 0 */
		if(l->d == -1) {
			l->d = 0;
			l->f = 0;
		}
		/* liste non vide : décalage de f sur la case suivante */
		else
			l->f = LISTE_SUIV(l->n, l->f);

		/* __ Insertion de l'élément en la nouvelle position f */
		l->tab[l->f] = elt;
		l->pos[elt] = l->f;
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: statut = %d, l->d = %d, l->f = %d\n", __func__, statut, l->d, l->f);
	#endif

	/* retour du statut de réussite ou d'échec (élément figurant déjà dans la liste) */
	return statut;
}

/* Cette fonction supprime et stocke dans elt l'élément en fin de liste si la liste n'est pas vide (auquel cas elle renvoie 0), elle renvoie -1 sinon
*/
int liste_supprimer_debut(liste* l, int* elt)
{
	int statut = -1;		/* statut de réussite (statut == 0) ou d'échec (statut == -1) de la fonction */

	#if(LISTE_DEBUG)
	printf("%s(l = %p) IN : l->d = %d, l->f = %d\n", __func__, (void*)l, l->d, l->f);
	#endif

	/* __ L'élément de tête ne peut être supprimé que si la liste n'est pas vide */
	if (! liste_est_vide(l)) {
		statut = 0;

		/* récupération de l'élément de début et mise à jour de sa position */
		*elt = l->tab[l->d];
		l->pos[*elt] = -1;

		/* __ mise à jour du curseur d
		*/

		/* liste résultante vide : d devient -1 */
		if(liste_get_taille(l) == 1)
			l->d = -1;
		/* liste résultante non vide : décalage de d sur la case suivante */
		else
			l->d = LISTE_SUIV(l->n, l->d);
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: statut = %d, l->d = %d, l->f = %d\n", __func__, statut, l->d, l->f);
	#endif

	/* retour du statut de réussite ou d'échec (liste vide) */
	return statut;
}

/* Cette fonction supprime et stocke dans elt l'élément en début de liste si la liste n'est pas vide (auquel cas elle renvoie 0), elle renvoie -1 sinon
*/
int liste_supprimer_fin(liste* l, int* elt) {
	int statut = -1;		/* statut de réussite (statut == 0) ou d'échec (statut == -1) de la fonction */

	#if(LISTE_DEBUG)
	printf("%s(l = %p) IN : l->d = %d, l->f = %d\n", __func__, (void*)l, l->d, l->f);
	#endif

	/* __ L'élément de queue ne peut être supprimé que si la liste n'est pas vide */
	if (! liste_est_vide(l)) {
		statut = 0;

		/* récupération de l'élément de fin et mise à jour de sa position */
		*elt = l->tab[l->f];
		l->pos[*elt] = -1;

		/* __ mise à jour des curseurs d et f
		*/

		/* liste résultante vide : d devient -1 */
		if(liste_get_taille(l) == 1)
			l->d = -1;
		/* liste résultante non vide : décalage de f sur la case précédente */
		else
			l->f = LISTE_PREC(l->n, l->f);
	}

	#if(LISTE_DEBUG)
	printf("%s OUT: statut = %d, l->d = %d, l->f = %d\n", __func__, statut, l->d, l->f);
	#endif

	/* retour du statut de réussite ou d'échec (liste vide) */
	return statut;
}

/* ______________________________________ Accesseurs en lecture
*/

/* Cette fonction renvoie l'entier n qui spécifie le domaine Z_n de ses éléments
	Pré-condition : *l variable liste
*/
int liste_get_n(liste* l) {
	return l->n;
}

/* Cette fonction renvoie 1 si la liste est vide et 0 sinon
*/
int liste_est_vide(liste* l) {
	return (l->d == -1);
}

/* Cette fonction renvoie 1 si la liste est pleine et 0 sinon
*/
int liste_est_pleine(liste* l) {
	return (liste_get_taille(l) == l->n);
}

/* Cette fonction renvoie la taille (nombre d'éléments qu'elle contient réellement) de la liste
*/
int liste_get_taille(liste* l) {
	int taille =0;

	/* la taille est non nulle si et seulement si la liste n'est pas vide */
	if (! liste_est_vide(l)) {
		/* la taille est (l->f -l->d +1) mod l->n */
		if (l->f >= l->d)
			taille = l->f -l->d +1;
		else
			taille = l->n +l->f -l->d +1;
	}

	return taille;
}

/* Cette fonction renvoie 1 si l'élement passé en argument figure dans la liste
*/
int liste_contient_element(liste* l, int elt) {
	return (l->pos[elt] != -1);
}

/* ______________________________________ Parcours de la liste
*/

/* Cette fonction renvoie l'adresse du premier élément de la liste si la liste n'est pas vide, NULL sinon
*/
int* liste_get_debut(liste* l) {
	return liste_get_suivant(l, NULL);
}

/* Cette fonction renvoie l'adresse de l'élement suivant elt dans la liste si un tel élément existe et NULL sinon, 
		où elt est soit NULL (auquel cas la fonction renvoie, si la liste n'est pas vide, l'adresse de son premier élément) ou l'adresse d'un élément de la liste.
*/
int* liste_get_suivant(liste* l, int* elt) {
	int* elt_next =NULL;

	/* Si elt == NULL : on renvoie s'il existe l'adresse du premier élément de la liste
	*/
	if (elt == NULL) {
		if (!liste_est_vide(l))
			elt_next =l->tab +l->d;
	}
	/* Sinon et si la liste n'est pas vide : on renvoie si cet élément existe l'adresse de l'élément suivant de la liste, ce qui est le cas si :
			l->d <= l->f		et elt <  tab +l->f (auquel cas l->d < l->f)		-> l'adresse suivante est alors elt +1
		ou 	l->f <  l->d		et elt <  tab +l->f 								-> l'adresse suivante est alors elt +1
		ou	l->f <  l->d		et elt->d <= elt <  tab +l->n					-> l'adresse suivante est alors l->tab si elt = tab +l->n -1 et elt +1 sinon
	*/
	else if (!liste_est_vide(l)) {
		if((elt -l->tab) < l->f)
			elt_next =elt +1;
		else if(l->f < l->d && (elt -l->tab) == l->n -1)
			elt_next =l->tab;
		else if(l->f < l->d && l->d <= (elt -l->tab) && (elt -l->tab) < l->n -1)
			elt_next =elt +1;
	}

	return elt_next;
}

/* Cette fonction renvoie l'adresse du dernier élément de la liste si la liste n'est pas vide, NULL sinon
*/
int* liste_get_fin(liste* l) {
	return liste_get_precedent(l, NULL);
}

/* Cette fonction renvoie l'adresse de l'élement précédent elt dans la liste si un tel élément existe et NULL sinon, 
		où elt est soit NULL (auquel cas la fonction renvoie, si la liste n'est pas vide, l'adresse de son dernier élément) ou l'adresse d'un élément de la liste.
*/
int* liste_get_precedent(liste* l, int* elt) {
	int* elt_next =NULL;

	/* Si elt == NULL : on renvoie s'il existe l'adresse du dernier élément de la liste
	*/
	if (elt == NULL) {
		if (!liste_est_vide(l))
			elt_next =l->tab +l->f;
	}
	/* Sinon et si la liste n'est pas vide : on renvoie si cet élément existe l'adresse de l'élément précédent de la liste, ce qui est le cas si :
			l->d <= l->f		et elt >  tab +l->d (auquel cas l->d < l->f)		-> l'adresse précédente est alors elt -1
		ou 	l->f <  l->d		et elt >  tab +l->d 								-> l'adresse précédente est alors elt -1
		ou 	l->f <  l->d		et elt <= tab +l->f 								-> l'adresse précédente est alors l->tab +l->n -1 si elt =l->tab et elt -1 sinon
	*/
	else if (!liste_est_vide(l)) {
		if((elt -l->tab) > l->d)
			elt_next =elt -1;
		else if(l->f < l->d && (elt -l->tab) == 0)
			elt_next =l->tab +l->n -1;
		else if(l->f < l->d && (elt -l->tab) <= l->f)
			elt_next =elt -1;
	}

	return elt_next;
}

/* ______________________________________ Entrées / sorties
*/

/* Cette fonction affiche une liste sur la console
*/
void liste_afficher(liste* l) {
	int* i;

	if(l == NULL)
		printf("Structure non instanciee (NULL).\n");
	else if(liste_est_vide(l))
		printf("Liste vide sur Z_%d.\n", liste_get_n(l));
	else {
		printf("Liste %s de %d elements sur Z_%d:\t( ", liste_est_pleine(l) ? "pleine" : "", liste_get_taille(l), liste_get_n(l));

		/* affichage du contenu de la liste */
		for (i = liste_get_debut(l) ; i != NULL ; i = liste_get_suivant(l, i))
			printf("%d ", *i);

		printf(")\n");
	}
}

