/*	liste : séquences d'entiers de Z_n étant donné un entier n >= 1 qui ne sont accessibles que par leur début ou leur fin :: fichier d'entête

	Contexte d'utilisation :
	- on dispose d'un ensemble V = {0, 1 ,.., n -1} de n éléments (les sommets d'un graphe)
	- on manipule des séquences gamma =(v_1, v_2,.., v_k) de sommets deux à deux distincts
	- on peut prolonger une séquence en y insérant un nouveau sommet, en début ou en fin de séquence
	- on peut écourter une séquence en en supprimant le premier / le dernier sommet
*/

#ifndef LISTE_H
#define LISTE_H

/* ______________________________________ Structure de données
*/

/* Liste par tableau:
	- les éléments de la liste sont stockés consécutivement dans un tableau tab d'une longueur donnée n ;
	- deux indices d et f spécifient où commence et où se termine la lecture des éléments de la liste ;
	- pour identifier une liste vide, on utilise le curseur d et la valeur -1.
	Ainsi : 
		- si d == -1 : la liste est vide
		- si d <> -1 et d <= f : la liste est (tab[d], tab[d +1] ,..., tab[f -1], tab[f])
		- si d <> -1 et f <  d : la liste est (tab[d], tab[d +1] ,..., tab[n -1], tab[0] ,..., tab[f -1], tab[f])

	Pour savoir en temps constant si un élément v de Z_n se trouve dans la liste, on maintient en plus un tableau des positions des éléments dans le tableau
*/
struct s_liste {
	int n;	/* spécifie l'ensemble de base Z_n des éléments de la liste et la capacité de la liste (nombre maximal d'éléments qu'elle peut contenir) */
	int* tab;	/* tableau dans lequel les éléments de la liste sont stockés consécutivement */
	int d;	/* curseur début de liste : indice à valeur dans {0, 1 ,..., n -1} si la liste n'est pas vide, de valeur n sinon */
	int f;	/* curseur fin de liste : indice à valeur dans {0, 1 ,..., n -1} si la liste n'est pas vide, de valeur quelconque sinon */
	int* pos;	/* tableau associant à chaque élément v de Z_n sa position dans tab (-1 si v ne figure pas dans la liste) */
};

typedef struct s_liste liste;

/* ______________________________________ Destruction / Construction / Initialisation
*/

/* Cette fonction construit, initialise et renvoie une liste vide de capacité n >= 1 en cas de succès, elle renvoie NULL en cas d'échec (allocation mémoire ou n <= 0)
*/
liste* liste_construire(int n);

/* Cette fonction construit et renvoie en cas de succès une copie d'une liste passée en argument ; en cas d'échec (allocation mémoire), elle renvoie NULL 
	Pré-condition : *l_source variable liste correctement instanciée 
*/
liste* liste_copie(liste* l_source);

/* Cette fonction vide une liste donnée
	Pré-condition : *l variable liste correctement instanciée
*/
void liste_vider(liste* l);

/* Cette fonction libère la mémoire allouée pour une liste (et réinitialise à NULL le pointeur *l)
	Pré-conditions : 
		- si *l <> NULL, alors **l variable liste allouée dynamiquement
		- si de plus *l->tab <> NULL (resp., *l->pos <> NULL), alors *l->tab (resp., *l->pos) tableau alloué dynamiquement
	Post-condition : si *l <> NULL, alors *l == NULL 
*/
void liste_detruire(liste** l);

/* ______________________________________ Insertion / Supression
*/

/* Cette fonction ajoute un élément donné en début de liste s'il ne s'y trouve pas déjà (auquel cas elle renvoie 0), elle renvoie -1 sinon
	Pré-condition : *l variable liste correctement instanciée 
*/
int liste_ajouter_debut(liste* l, int elt);

/* Cette fonction ajoute un élément donné en fin de liste s'il ne s'y trouve pas déjà (auquel cas elle renvoie 0), elle renvoie -1 sinon
	Pré-condition : *l variable liste correctement instanciée 
*/
int liste_ajouter_fin(liste* l, int elt);

/* Cette fonction supprime et stocke dans elt l'élément en fin de liste si la liste n'est pas vide (auquel cas elle renvoie 0), elle renvoie -1 sinon
	Pré-condition : *l variable liste correctement instanciée 
*/
int liste_supprimer_debut(liste* l, int* elt);

/* Cette fonction supprime et stocke dans elt l'élément en début de liste si la liste n'est pas vide (auquel cas elle renvoie 0), elle renvoie -1 sinon
	Pré-condition : *l variable liste correctement instanciée 
*/
int liste_supprimer_fin(liste* l, int* elt);

/* ______________________________________ Accesseurs en lecture
*/

/* Cette fonction renvoie l'entier n qui spécifie le domaine Z_n de ses éléments
	Pré-condition : *l variable liste
*/
int liste_get_n(liste* l);

/* Cette fonction renvoie 1 si la liste est vide et 0 sinon
	Pré-condition : *l variable liste
*/
int liste_est_vide(liste* l);

/* Cette fonction renvoie 1 si la liste est pleine et 0 sinon
	Pré-condition : *l variable liste
*/
int liste_est_pleine(liste* l);

/* Cette fonction renvoie la taille (nombre d'éléments qu'elle contient réellement) de la liste
	Pré-condition : *l variable liste
*/
int liste_get_taille(liste* l);

/* Cette fonction renvoie 1 si l'élement passé en argument figure dans la liste
	Pré-condition : *l variable liste correctement instanciée 
*/
int liste_contient_element(liste* l, int elt);

/* ______________________________________ Parcours de la liste
*/

/* Cette fonction renvoie l'adresse du premier élément de la liste si la liste n'est pas vide, NULL sinon
	Pré-condition : *l variable liste
*/
int* liste_get_debut(liste* l);

/* Cette fonction renvoie l'adresse de l'élement suivant elt dans la liste si un tel élément existe et NULL sinon, où elt est soit NULL (auquel cas la fonction renvoie, si la liste n'est pas vide, l'adresse de son premier élément), soit l'adresse d'un élément de la liste.
	Pré-condition : 
	- *l variable liste, 
	- elt =NULL ou dans {l->t +l->d ,.., l->t +l->f} si l->d <= l->f, dans {l->t +l->d ,.., l->t +l->n -1} U {l->t ,.., l->t +l->f} sinon 
*/
int* liste_get_suivant(liste* l, int* elt);

/* Cette fonction renvoie l'adresse du dernier élément de la liste si la liste n'est pas vide, NULL sinon
*/
int* liste_get_fin(liste* l);

/* Cette fonction renvoie l'adresse de l'élement précédent elt dans la liste si un tel élément existe et NULL sinon, où elt est soit NULL (auquel cas la fonction renvoie, si la liste n'est pas vide, l'adresse de son dernier élément), soit l'adresse d'un élément de la liste.
	Pré-condition : 
	- *l variable liste, 
	- elt =NULL ou dans {l->t +l->d ,.., l->t +l->f} si l->d <= l->f, dans {l->t +l->d ,.., l->t +l->n -1} U {l->t ,.., l->t +l->f} sinon 
*/
int* liste_get_precedent(liste* l, int* elt);

/* ______________________________________ Entrées / sorties
*/

/* Cette fonction affiche une liste sur la console
	Pré-condition : si l <> NULL, *l variable liste correctement instanciée 
*/
void liste_afficher(liste* l);

#endif

