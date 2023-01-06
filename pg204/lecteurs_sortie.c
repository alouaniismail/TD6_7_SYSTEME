#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "utils.h"

#define POOL 2
#define SIZE 10000
#define LOOP 10

pthread_mutex_t m1;
pthread_mutex_t m2;

/*opaque function*/
int consommer(int* value){
  usleep(10);
  return *value;
}

/*opaque function*/
void produire(int v, int* value){
  usleep(1);
  (*value)+=v;
}

/*
normalement:
le consommateur doit consommer pas le vide et lorsque le producteur va finir
et le producteur doit pouvoir ecrire dans un espace vide sans que
le consommateur intervient
*/

int A[SIZE];
int sum;

//rappel: memset(void*, const void*, size_t bytes).
//(rapide pour mettre tous les elmts d'un tableau a 0 au lieu d'une boucle)

void* producteur(void* ptr){
  int i,j;
  pthread_mutex_lock(&m1);//bloquer l'accès au lecteur.
  for(i=0;i<LOOP;i++){
    for(j=0;j<SIZE;j++){
      //pas d'accès aux données manipulées.
      produire(1,A+j);
    }
  }
  pthread_mutex_unlock(&m1);
  printf("J'ai fini d'ecrire\n");
  return NULL;
}

void* consommateur(void* ptr){
  int i,j;
  pthread_mutex_lock(&m1);//pas accès au données jusqu'à unlock(&m1).
  //pthread_mutex_lock(&m2);

  //ce lock indique qu'on a posé un verrou sur ce qui suit via m1
  //qui est déjà déposé sur producteur
  //et encadré
  //ce qui implique qu'on l'a pas unlock
  //donc pas accès aux données à l'intèrieur jusqu'à unlock.
  
  for(i=0;i<LOOP;i++){
    for(j=0;j<SIZE;j++){
      sum+=consommer(A+j);
    }
  }
  //pthread_mutex_unlock(&m1);
  return NULL;
}

//produire et consommer sont en parallele.
//Remarque: 1 fonction via 2 threads=> acces concurrent =>1 mutex suffit
//pour l'ordonnanceur.
//2 fonctions via 2 threads linkage oui => 2 mutex surtout dans cette situation
//remplissage & calcul de la somme qui doit attendre le remplissage
//entier du buffer dans le schema classique d'ecrivain, lecteur.

int main(int argc, char** argv){
  memset(A,0,SIZE*sizeof(int));
  sum=0;
  pthread_mutex_init(&m1,NULL);
  pthread_mutex_init(&m2,NULL);
  pthread_t t1;
  pthread_t t2;
  pthread_create(&t1,NULL,producteur,NULL);
  pthread_create(&t2,NULL,consommateur,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  printf("La somme est de %d et devrai être de %d\n",sum,SIZE*10*10);
  return EXIT_FAILURE;
}
  
/*
problème:
le producteur ecrit et et mem temps a un certain moment, le t2 se lance
calcule la somme avec les donnes incompletes de t1(producteur function)
A n'est pas mise à 100%, du coup le calcul est moindre t1 quitte t2 quitte
avant le main thread quitte et on se retrouve avec une somme pas correct.
*/

//en général, on a utilisé en gros 1 seul mutex<=>comportement de sémaphore
//=>solution_choisie.c.
