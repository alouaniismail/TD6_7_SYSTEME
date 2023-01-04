#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<pthread.h>

#include "utils.h"

void* start(void* arg){
  intptr_t pro_id=(intptr_t) arg;
  //rappel rapide: uint8_t /../ uint64_t
  //est un entier non signé codé sur ?? bits.

  //normal, passons au intptr_t qui  est an integer's value.
  //trop grand pour contenir une adresse void*. C'est ça l'idée.
  
  printf("Processus ID %d, thread ID %p=%d\n",getpid(),&pro_id,(int)pro_id);

  //on affiche le pid, le thread avec l'adresse de pro_id=(intptr_t)arg
  //=%d avec ce dernier (int)pro_id (conversion: intptr_t->int).
  //intptr_t->void* possible c'est pour cela: intptr_t (entier signe!).
  
  return NULL; //pas important.
}

int main(int argc, char* argv[]){
  int i;
  pthread_t threads[7];
  //initialisation naive de 7 threads.

  for(i=0;i<7;i++){//0->6: 7 tours de boucle.
    pthread_create(&threads[i],NULL,start,(void*)((intptr_t)i));
  }
  //avec comme argument (void*)((intptr_t)i).

  for(i=0;i<7;i++){//de même.
    pthread_join(threads[i],NULL);
  }
  //affichage normal
  //voilà.
  return EXIT_SUCCESS;
}
