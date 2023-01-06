#include <stdio.h>
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h>

#define N 5  //nbr de philosophes        
#define G (i+1)%N   //fourchette gauche du philosophe i
#define D i         //fourchette droite du philosphe i
#define libre 1 
#define occupe 0 

void* philosophe(void* num);//déclaration de la fon void*()(void*).

int fourch[N] = { libre, libre, libre, libre, libre }; 
sem_t mutex;



int main () { 
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
    sem_wait(&mutex); //essayer de prendre les fourchettes pour manger
    //bloquer le fait de faire cette action de la part des autres philosophes.
    if(fourch[G] == libre && fourch[D] == libre) { 
            fourch[G] = occupe; 
	    fourch[D] = occupe; 
	    sem_post(&mutex);//toutes les fourchettes sont occupees.
	    nb--; 
	    printf("philosophe [%d] mange\n", i); 
	    sleep (1); //manger           
	    printf("philosophe [%d] a fini de manger\n", i); 

	    sem_wait(&mutex);//libérer les fourchettes   
	    //bloquer car il se peut qu'un philosophe dès que c'est libre
	    //prend les deux fourchettes alors qu'on est à l'étape de la 1ère
	    //libération: problème.
	    fourch[G] = libre; 
	    fourch[D] = libre; 
	    sem_post(&mutex);//donner la main à l'ordonnanceur: fin.
 
    } else
      sem_post(&mutex);
  }
}

  //si occupation: ne pas essayer de prendre
    //libérer le 'sémaphore'. 



/*
situation à gérer: fourchette->baguette:
Cinq philosophes sont assis autour d’une table. Sur la table, il y a 
alternativement 5 plats de spaghettis et 5 fourchettes. 
●
Un philosophe passe son temps à manger et à penser. 
●
Pour manger son plat de spaghettis, un philosophe a besoin de 
deux fourchettes qui sont de part et d’autre de son plat. 
●
Si tous les philosophes prennent en même temps, chacun une 
fourchette, aucun d’entre eux ne pourra prendre l’autre fourchette 
(situation d’interblocage). 
●
Pour éviter cette situation, un philosophe ne prend jamais une 
seule fourchette.   
●
Les fourchettes sont les objets partagés. L’accès et l’utilisation 
d’une fourchette doivent se faire en exclusion mutuelle. On utilisera 
le sémaphore mutex pour réaliser l’exclusion mutuelle. 
*/
