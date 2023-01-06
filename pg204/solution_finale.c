#include <stdio.h>
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h>

#define N 5  //nbr de philosophes        
#define G (N+i-1)%N   //baguette partie gauche du philosophe i
#define D i         //baguette partie droite du philosphe i
#define libre 1 
#define occupe 0 

void* philosophe(void* num);//déclaration de la fon void*()(void*).

int baguette[N] = { libre, libre, libre, libre, libre }; 
sem_t mutex;



int main (int argc, char* argv[]) { 
  int i, NumPhi[N] = { 0, 1, 2, 3, 4 };

  pthread_t th[N]; 

  sem_init (&mutex, 0, 1); 

  //création des n philosophes.
  for (i = 0; i < N; i++) 
    pthread_create (&th[i], NULL, philosophe, &NumPhi[i]); 

  //attendre la fin des threads
  for (i = 0; i < N; i++) 
    pthread_join (th[i], NULL); 

  printf ("fin des  threads \n"); 

  return 0; 
}

//le main est classique, il initialise les choses normalement avec le sem_t.

void* philosophe (void * num) {
  int i = *(int*) num, nb = 2;
  while (nb) { 
    sleep (1);//penser           
    sem_wait(&mutex); //essayer de prendre les baguettes pour manger
    //bloquer le fait de faire cette action de la part des autres philosophes.
    if(baguette[G] == libre && baguette[D] == libre) { 
            baguette[G] = occupe; 
	    baguette[D] = occupe; 
	    sem_post(&mutex);//toutes les baguettes(parties) sont occupees.
	    nb--; 
	    printf("philosophe [%d] mange\n", i); 
	    sleep (1); //manger           
	    printf("philosophe [%d] a fini de manger\n", i); 

	    sem_wait(&mutex);//libérer les baguetteettes   
	    //bloquer car il se peut qu'un philosophe dès que c'est libre
	    //prend les deux baguettes(partie) alors qu'on est à l'étape de la 1ère
	    //libération: problème.
	    baguette[G] = libre; 
	    baguette[D] = libre; 
	    sem_post(&mutex);//donner la main à l'ordonnanceur: fin.
 
    } else sem_post(&mutex); //si occupation: ne pas essayer de prendre
    //libérer le 'sémaphore'.
  }
}

