#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<pthread.h>

#include "utils.h"

#define POOL 8

pthread_mutex_t fastmutex=PTHREAD_MUTEX_INITIALIZER;

int nbtickets=1000;

void* vendeur(void* arg){
  int nbtickets_vendus=0;
  intptr_t id=(intptr_t) arg;


  
  while(1){
    pthread_mutex_lock(&fastmutex);
    
    if(nbtickets<=0) {
      pthread_mutex_unlock(&fastmutex);
      break;
    }
    
    nbtickets--;//accès via plusieurs threads on l'encadre avec un mutex(fastmutex)
    //initialisation via: fastmutex=PTHREAD_MUTEX_INITIALIZER; le type est pthread_mutex_t.

    
    pthread_mutex_unlock(&fastmutex);
    nbtickets_vendus++;//ça importe peu, c'est local à chaque thread, c'est pas une variable globale.

    
    sched_yield();
    //donner la main à une chance d'autre processus via le CPU dans la liste des prioritées.

    /*avec le while(1) c'est fort et ca se mène bien.*/

    
  }
    printf("[%d] J'ai vendu %d tickets \n",(int)id,nbtickets_vendus);
    return (void*)((intptr_t)nbtickets_vendus);
}


int main(int argc, char* argv[]){//le main est clair, la variable globale et le sens de la fonction sont comprises, incluses dans la totalitée du deuxième fichier gestion sans mutex.

  
  int nbtickets_vendus=0;
  if(argc>=2){
    nbtickets=atoi(argv[1]);
  }
  int i;
  pthread_t threads[POOL];
  for(i=0;i<POOL;i++){
    pthread_create(&threads[i],NULL,vendeur,(void*)((intptr_t)i));
  }
  intptr_t thread_return;
  for(i=0;i<POOL;i++){
    pthread_join(threads[i],(void**)&thread_return);
    nbtickets_vendus+=(int)thread_return;
  }

  pthread_mutex_destroy(&fastmutex);
  printf("%d tickets ont été vendus (nbtickets=%d)\n",nbtickets_vendus,nbtickets);
  return EXIT_SUCCESS;
}

/*
là tout marche, le parallélisme conduit à un accès concurrent à la même variable ; c'est possible du coup on fait quoi ?

=>On l'encadre avec lock/unlock de mutex.(pthread_mutex_t) &fastmutex de valeur sans &: PTHREAD_MUTEX_INITIALIZER.

en général, pour les variables globales avec accès via plusieurs threads(fonctions) => encadrement via mutex pour un fonctionnement cohérent totalement.





*/
