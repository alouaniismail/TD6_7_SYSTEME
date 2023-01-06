#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#include "utils.h"

//lorsque différents threads opérent sur un segment partagé différemment,
//il est préférable d'utiliser les mutex. 

#define NTHREADS 2
sem_t semaphore;

void *avant(void * param) {
  //première fonction à faire(ordre:2ième but des semaphores)

  // do something
  for(int j=0;j<1000000;j++) {
  }//tjrs sem_wait fait attendre ensuite.
  sem_post(&semaphore);//debloquage.
  return(NULL);
}

void *ensuite(void * param) {
  sem_wait(&semaphore);//problème: à 0 donc bloque ici jusqu'à le lancement
  //de avant.
  
  // do something

  //lancement de ensuite après avoir fini de avant.
  for(int j=0;j<1000000;j++) {
  }
  return(NULL);
}

int main (int argc, char *argv[])  {
  pthread_t thread[NTHREADS];
  void * (* func[])(void *)={avant, ensuite};
  int err;

  err=sem_init(&semaphore, 0,0);
  if(err!=0) {
      exit_if(err,"sem_init");
  }
  for(int i=0;i<NTHREADS;i++) {
    err=pthread_create(&(thread[i]),NULL,func[i],NULL);
    if(err!=0) {
      exit_if(err,"pthread_create");
    }
  }

  for(int i=0;i<NTHREADS;i++) {
    err=pthread_join(thread[i],NULL);
    if(err!=0) {
      exit_if(err,"pthread_join");
    }
  }
  sem_destroy(&semaphore);
  if(err!=0){//la fin et la référence(C++).
      exit_if(err,"sem_destroy");
  }
  return(EXIT_SUCCESS);
}

/*
donner l'idée d'améliorer voire de sauver solution_1 en solution_2
puis la valider en cp .. solution_choisie
*/
