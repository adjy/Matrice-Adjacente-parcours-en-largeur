/* ----------------------------------------------------------------- 
 * définition des structures  nécessaire pour représenter une matrice
 * d'adjacence sous forme de matrice creuse.
 * -----------------------------------------------------------------
 */

/*
 * structure d'un maillon de la liste chaînée associé 
 * à chaque ligne de la matrice
 */
struct Maillon {
  int col; // numéro de la colonne à laquelle correspond le coefficient
  int coef;// coefficient de la matrice
  Maillon *suiv;// élément suivant non nul sur la ligne
} ;

/*
 * structure représentant une matrice d'adjacence sous forme
 * de matrice creuse. La structure contient un tableau de lignes, chaque
 * ligne étant rerpésentée par une liste chaînée de "maillons".
 */
struct MatriceAdjacence {
  int ordre; // nombre de sommets du graphe
  Maillon* *lignes;// tableau à allouer de taille "ordre", représentant les lignes de la matrice
};


/* ----------------------------------------------------------------- 
 * définition des structures  nécessaire pour représenter une liste
 * doublement chaînée de nombre entiers
 * -----------------------------------------------------------------
 */

/*
 * structure d'un maillon de liste chaînée d'entiers 
 */
struct MaillonEntier {
  int valeur; // valeur stockée dans le maillon
  MaillonEntier *suiv = nullptr;// élément suivant dans la file
  MaillonEntier *prec = nullptr;// élément précédent dans la file
} ;

/*
 * structure d'une file de type FIFO pouvant contenir des entiers
 */
struct Fifo {
  MaillonEntier *in;
  MaillonEntier *out;
};


/*
 * type énuméré permettant de représenter les couleurs
 */
enum Couleur {BLANC, GRIS, NOIR};

// valeur indéfinie pour un indice de parent
#define INDEFINI  -1

// valeur infinie pour une distance enre sommets
#define INFINI  -1
