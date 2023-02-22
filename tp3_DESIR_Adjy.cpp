#include <iostream>
#include <fstream>
using namespace std;

#include "types.hpp"

/*
    DESIR Adjy Sedar
    Groupe 2
    Licence 2 Info
*/

/* pré-définition des fonctions utilisées dans ce module
 * Le code des fonctions est donnée après la fonction main
 */
void creerMatrice(MatriceAdjacence &m, int taille);
void effacerMatrice(MatriceAdjacence &mat);
void afficher(MatriceAdjacence mat);
bool charger(char *nom, MatriceAdjacence &mat);
void initialiserFifo(Fifo &file);
bool estVide(Fifo file);
void ajouter(Fifo &file, int v);
void afficherListe(Fifo &file);
int retirer(Fifo &file);
void parcoursEnLargeur(MatriceAdjacence mat, int sommetDepart,
Couleur *coul, int *dist, int *parent);
void affCoul(Couleur *coul, int taille);
void affPar(int *parent, int taille);
void affDist(int *distance, int taille);
void afficherCheminVers(int sf, int *parent);

int main(int argc, char *argv[]){
  
  if(argc!=3){
    cout << "Erreur - il manque le nom du fichier à lire ou la matrice de depart\n";
    return -1;
  }

  MatriceAdjacence mat = {0, nullptr};
  

  if(!charger(argv[1], mat)) 
    return -1;

  if( (atoi(*(argv + 2)) >= mat.ordre) || (atoi(*(argv + 2)) < 0) ){
    cout<<"indice de sommet "<<atoi(*(argv + 2))<< " incorrect ! valeurs autoris´ees dans [0,"<<mat.ordre - 1<<"]";
    return -1;
  }

  afficher(mat);

   Couleur *coul = new Couleur [mat.ordre];
  int *dist = new int[mat.ordre];
  int *parent = new int[mat.ordre];

  /*  Couleur coul[mat.ordre];
  int dist[mat.ordre];
  int parent[mat.ordre];*/
 

  parcoursEnLargeur(mat, atoi(*(argv + 2)), coul, dist, parent);
  
  
  cout<<endl;
  affCoul(coul, mat.ordre);
  affDist(dist, mat.ordre);
  affPar(parent, mat.ordre);
  cout<<endl;


  for(int i = 0; i<mat.ordre; i++){
    if( i == atoi(*(argv + 2)))
      continue;
    if(*(parent + i) == INDEFINI)
      cout<<"Pas de chemin de "<<atoi(*(argv + 2))<<" vers " <<i;
    else{
      cout<<"Chemin vers "<<i<<" = ";
      afficherCheminVers(i, parent);
    }
    cout<<endl;
  }
   

  
  effacerMatrice(mat);
  

  return 1;
}


void creerMatrice(MatriceAdjacence &m, int taille){
  // raz éventuelle de la matrice
  if(m.lignes!=nullptr) delete m.lignes;
  // initialisation du nombre de lignes/colonnes de la matrice
  m.ordre = taille;
  // allocation mémoire du tableau de lignes
  m.lignes = new Maillon*[taille];
  // initialisation de chaque ligne à "vide"
  for(int i=0; i<taille; i++) m.lignes[i]=nullptr;
}

void effacerMatrice(MatriceAdjacence &mat){
  for(int l=0; l<mat.ordre; l++){// effacer chaque ligne
    while(mat.lignes[l]!=nullptr){// tq la ligne n'est pas vide
      // effacer le premier élément qui s'y trouve
      Maillon *cour = mat.lignes[l];// 1er élément de la liste
      mat.lignes[l] = cour->suiv;// élément suivant éventuel
      delete cour; // effacer le 1er élement courant
    }
  }
  // effacer le tableau de lignes
  delete mat.lignes;
  mat.lignes = nullptr;
  // raz de la taille
  mat.ordre = 0;
      
}

bool charger(char *nom, MatriceAdjacence &mat){
  ifstream in;
  
  in.open(nom, std::ifstream::in);
  if(!in.is_open()){
    printf("Erreur d'ouverture de %s\n", nom);
    return false;
  }

  int taille;
  in >> taille;



  // créer la matrice
  creerMatrice(mat, taille);
  
  int v; // coefficient lu

  for(int l=0; l<mat.ordre; l++){ // lire et créer une ligne complète
    Maillon *fin=nullptr;// pointeur vers la fin d'une liste chaînée
    for(int c=0; c<mat.ordre; c++){ // lire et créer chaque colonne de la ligne courante
      in >> v;// lecture du coefficient (0 ou 1)
      if(v!=0){// créer un maillon et l'insérer en fin de liste
	// créer un nouveau maillon
	Maillon *nouveau = new Maillon;
	nouveau->col = c;
	nouveau->coef = v;
	nouveau->suiv = nullptr;
	// insérer le maillon en fin de liste
	if(fin!=nullptr){// il y a déjà des éléments dans la liste
	  fin->suiv = nouveau;// insertion en fin
	  fin = nouveau;// maj du pointeur vers le dernier élément de la liste
	}else{// c'est le premier coefficient de la liste
	  mat.lignes[l] = nouveau;// ajout au début de la liste
	  fin = nouveau;// maj du pointeur vers le dernier élément de la liste
	}
      }// if - rien à faire si v vaut 0
    }// for c
  }// for l

  in.close();
  return true;
}

void afficher(MatriceAdjacence mat){
  // affichage de chacune des lignes
  for(int l=0; l<mat.ordre; l++){// affichage de la ligne l
    int c=0;
    Maillon *mcur=mat.lignes[l];
    while(c<mat.ordre){
      if(mcur==nullptr){// le coefficients de la ligne >=c sont nuls
	cout << "0 ";
	c++;
      }else if(mcur->col != c){
	// on est sur un coefficient nul, qui se trouve avant c
	cout << "0 ";
	c++;
      }else{// afficher le coefficient
	cout << mcur->coef << " ";
	mcur = mcur->suiv;
	c++;
      }   
    }// while
    cout << endl;// fin de la ligne l
  }// for
}

void initialiserFifo(Fifo &file){
  file.in = nullptr;
  file.out = nullptr;
}

bool estVide(Fifo file){
  return (file.in == nullptr && file.out == nullptr);
}

void ajouter(Fifo &file, int v){
  MaillonEntier *maillon = new MaillonEntier;

  maillon->valeur = v;
  maillon->suiv = nullptr;
  maillon->prec = nullptr;
  
  // terminer l'initialisation ...
  if(estVide(file)){
    file.in = maillon;
    file.out = maillon;
    return;
  }

  maillon->suiv = file.in;
  file.in->prec = maillon;
  file.in = maillon;
    
}
void afficherListe(Fifo &file){
  for(MaillonEntier *ptr = file.in; ptr != nullptr; ptr = ptr->suiv)
    cout<<ptr->valeur<<" ";
  cout<<endl;
}

int retirer(Fifo &file){

  if(file.out != nullptr){
    MaillonEntier *aSupp = file.out;
    int val = aSupp->valeur;

    if( file.out->prec != nullptr){
      file.out->prec->suiv = nullptr;
      file.out = file.out->prec;
    }
    else{
      file.out = nullptr;
      file.in = nullptr;
    }
  delete aSupp;
  return val;
  }
  return -1;
  
}

void parcoursEnLargeur(MatriceAdjacence mat, int sommetDepart,
Couleur *coul, int *dist, int *parent){
  for(int i=0; i < mat.ordre; i++){
    *(dist + i) = INFINI;
    *(parent + i) = INDEFINI;
    *(coul + i) = BLANC;
  }

  /*init */
  *(dist + sommetDepart) = 0;
  *(parent + sommetDepart) = INDEFINI;
  *(coul + sommetDepart) = GRIS;
  Fifo mafile;
  initialiserFifo(mafile);
  ajouter(mafile, sommetDepart);
  
  

  while(!estVide(mafile)){

    int val = retirer(mafile);
    
    if(val != -1){
      for(Maillon *ptr = mat.lignes[val]; ptr != nullptr; ptr = ptr->suiv){
        int adj = ptr->col;
        if(*(coul + adj) == BLANC){
          *(coul + adj) = GRIS;
          *(dist + adj) = *(dist + val) + 1;
          *(parent + adj) = val;
          ajouter(mafile, adj);
        }
      }
      *(coul + val) = NOIR;
    }

  }
 


}

void affCoul(Couleur *coul, int taille){
  cout<<"Couleurs : ";
  for(int i=0; i<taille; i++){
    if(*(coul + i) == BLANC)
      cout<<"B";
    if(*(coul + i) == NOIR)
      cout<<"N";
    if(*(coul + i) == GRIS)
      cout<<"G";
    cout<<" ";
  }
  cout<<endl;
    
}


void affPar(int *parent, int taille){
  cout<<"Parents  : ";
  for(int i=0; i<taille; i++){
    if(*(parent + i) == INDEFINI)
      cout<<"X";
    else
      cout<<*(parent + i);
    cout<<" ";
  }
  cout<<endl;
}
void affDist(int *distance, int taille){
  cout<<"Distances: ";
  for(int i=0; i<taille; i++){
    if(*(distance + i) == INFINI)
      cout<<"X";
    else
      cout<<*(distance + i);
    cout<<" ";
  }
  cout<<endl;
}

void afficherCheminVers(int sf, int *parent){
  
  if(sf == INDEFINI)
    return;
  
  afficherCheminVers(parent[sf],parent);
  cout<<sf<<" ";

  

}
