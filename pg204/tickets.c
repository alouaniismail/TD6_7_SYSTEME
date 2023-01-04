#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "utils.h"

#define POOL 8

int nbtickets = 1000;

int vendeur( int id )
{
    int nbtickets_vendus = 0;
    
    while ( nbtickets > 0 ) {
	nbtickets_vendus++;
	sched_yield(); /* Do not touch this line */
	nbtickets--;
    }
    
    printf( "[%d] J'ai vendu %d tickets \n",
	    id, nbtickets_vendus );
    
    return nbtickets_vendus;
}

int
main( int argc, char *argv[] ) {
    int nbtickets_vendus = 0;

    if ( argc >= 2 ) {
	nbtickets = atoi( argv[1] );
    }

    nbtickets_vendus = vendeur( 0 );
    
    printf( "%d tickets ont été vendus (nbtickets = %d)\n",
	    nbtickets_vendus, nbtickets );
    
    return EXIT_SUCCESS;
}
