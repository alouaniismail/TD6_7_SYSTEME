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

/*
pthread_mutex_t m1;
pthread_mutex_t m2;
*/

sem_t semaphore1;
/*sem_t semaphore2;*/

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


int A[SIZE];
int sum;

/*
selon les deux fichiers .c et .txt crees apres avoir analyse le probleme,
l'ordre ici intervient et deux threads manipulent pratiquement les mêmes 
data=>implication de mutex. Le lecteur va attendre l'ecrivain on fait
l'initialisation du semaphore a 0 puis un sem_wait sur consommateur
puis un sem_post sur l'ecrivain tout à la fin.
là, on a fait différemment, on a initialisé à 1, puis wait sur lecrivain et le
lectuer semaphore à 0=>~bloquage du lecteur puis sem_post à la fin de l'ecrivain
qui debloque l'execution du consommateur
*/ 

void* producteur(void* ptr){
  int i,j;
  //semaphore devient 0 wait pour les autres threads/proces
  for(i=0;i<LOOP;i++){
    for(j=0;j<SIZE;j++){
      produire(1,A+j);
    }
  }
  sem_post(&semaphore1);//sem_post debloque : A est bien remplie.
  //le consommateur(thread) va faire sem_wait pour revenir à 0 et exécuter.
  //mais important que cette instruction réside elle bloque l'accès en parallèle
  //à consommatuer lorsque producteur se lancerait. 
  printf("J'ai fini d'ecrire\n");
  return NULL;
}

void* consommateur(void* ptr){
  int i,j;
  sem_wait(&semaphore1);//si producteur se lance
  //et consonmmateur intervient il y a sem_wait qui va décrementer
  //du coup:pas de sortie.
  for(i=0;i<LOOP;i++){
    for(j=0;j<SIZE;j++){
      sum+=consommer(A+j);
    }
  }
  return NULL;
}


int main(int argc, char** argv){
  memset(A,0,SIZE*sizeof(int));
  sum=0;
  /*
  pthread_mutex_init(&m1,NULL);
  pthread_mutex_init(&m2,NULL);
  */
  int err1=sem_init(&semaphore1,0,0);
  /*  int err2=sem_init(&semaphore2,0,0);*/
  pthread_t t1;
  pthread_t t2;
  pthread_create(&t1,NULL,producteur,NULL);
  pthread_create(&t2,NULL,consommateur,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  printf("La somme est de %d et devrai être de %d\n",sum,SIZE*10*10);
  sem_destroy(&semaphore1);
  /* sem_destroy(&semaphore2);*/
  return EXIT_FAILURE;
}
  
