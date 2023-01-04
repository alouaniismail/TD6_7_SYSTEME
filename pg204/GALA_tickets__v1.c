#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<pthread.h>

#include "utils.h"

#define POOL 8

int nbtickets=1000;//à vendre ceux-là.

void* vendeur(void* arg){

  //fonction de vente pour chaque élément en parallèle élève.

  int nbtickets_vendus=0;

  /*
    vendus = 0 tout au début.
    il faut jouer sur nbtickets en le decrementant.
  */
  
  intptr_t id=(intptr_t)arg;
  //identifiant du thread via: void* arg->(intptr_t)arg.


  while(1){
    if(nbtickets<=0)
      break;
    
    nbtickets--;//on décremente le nbr de tickets disponibles.
    nbtickets_vendus++;//on incrémente le nbr présentant ceux vendus.


    sched_yield();
//forcer le thread appelant à libérer le CPU. Le thread est déplacé à la fin de la liste des processus prêts de sa priorité et un autre thread sera exécuté.(l'idée est qu'on ait le mot printf une seule fois pour chaque id (intptr_t)arg de vendeur élève.) 
  }

  printf("[%d] J'ai vendu %d tickets \n",(int)id,nbtickets_vendus);
  /*
nbtickets_vendus localement à cet élève (int)id.
  */

  
  return (void*)((intptr_t)nbtickets_vendus);
  //on retourne un pointeur polymorphe vers le nbtickets_vendus.
}

int main(int argc, char* argv[]){
  int nbtickets_vendus=0;
  //initialiser à 0. (### spécialitée de 0 pour chaque vendeur(thread)).

  int i;
  pthread_t threads[POOL];
  for(i=0;i<POOL;i++){
    
    //pthread_create->on passe en 1er argument l'adresse(bcp d'arguments)
    //(moyen mnémotechnique)
    //et pthread_join non.

    //l'identifiant sera tout simplement le tour de boucle
    //de 0->7(8 élèves).


    pthread_create(&threads[i],NULL,vendeur,(void*)((intptr_t)i));
  }

  intptr_t thread_return;
  for(i=0;i<POOL;i++){
    //&thread_return peut être assimilé à un int*
    //qui fait référence selon la fonction void*(*vendeur)(void*)
    //le nbtickets_vendus de cet personne.(thread<=>.)

    //on passe le retour du thread en un void**
    //&thread_join est de type adresse -> on passe (void**)&thread_return
    //via la règle: intptr_t->void* en typecast direct
    //double on passe l'adresse de la valeur du type intptr_t.

    
    pthread_join(threads[i],(void**)&thread_return);
    nbtickets_vendus+=(int)thread_return;//on fait la somme
    //de tous les tickets vendus.(le retour: (void*)((intptr_t)??), le nbr de tickets vendus.


    
  }

  printf("%d tickets ont été vendus (nbtickets=%d)\n",nbtickets_vendus,nbtickets);

  //nbtickets_vendus en total c'est bon.
  //nbtickets en global dispos normalement nul si on vend tout.
  return 0;



}

/*
C'est ça le problème, on retrouve: 1675 vendus.
Pourquoi? 
=>problème de l'exclusion mutuelle!(concurrence)
*/
