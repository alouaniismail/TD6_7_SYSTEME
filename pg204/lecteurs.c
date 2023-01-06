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

int consommer(int* value){
  usleep(10);
  return *value;
}

void produire(int v, int* value){
  usleep(1);
  (*value)+=v;
}

int A[SIZE];
int sum;

//producteur -> ecrivain.

void* producteur(void* ptr){
  int i,j;
  for(i=0;i<LOOP;i++){//10
    for(j=0;j<SIZE;j++){//10000
      produire(1,A+j);//A[j]+=1.(remplir tous le tableau de 10(LOOP))
    }
  }
  printf("J'ai fini d'ecrire\n");
  return NULL;
}

void* consommateur(void* ptr){
  int i,j;
  //le tableau est rempli de 10.
  for(i=0;i<LOOP;i++){//10^5 10 fois => 10*10^5=1 million.
    for(j=0;j<SIZE;j++){//10000*10=10^5.
      sum+=consommer(A+j);
    }
  }
  return NULL;
}
      
int main(int argc, char** argv){
  memset(A,0,SIZE*sizeof(int));
  sum=0;
  pthread_t t1;
  pthread_t t2;
  pthread_create(&t1,NULL,producteur,NULL);
  pthread_create(&t2,NULL,consommateur,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  /*
  producteur(NULL);
  consommateur(NULL);
  */
  //la vraie somme sera de: sortie 1->tableau rempli de 10
  //puis: 10*SIZE*LOOP=10*2*10000=200000.
  printf("La somme est de %d et devrai être de %d\n",sum,SIZE*10*10);
  return EXIT_FAILURE;//10000*10^2=10^6.
}
  
