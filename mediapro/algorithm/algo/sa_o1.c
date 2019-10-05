//! gcc -o sa_o1.bin sa_o1.c -W -Wall -std=gnu99 -O3 -g
/* -*-coding: utf-8 -*-
 * sa_example2.c
 * 
 * if you can't read the following, please consult me.
 * この文字が無事読めたら良し
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * S;

/* int int_cmp(const void * a_, const void * b_) { */
/*     const int * a = a_; */
/*     const int * b = b_; */
/*     return strcmp(S + *a, S + *b); */
/* } */
void *mymalloc( size_t size, char *errmsg )
{
    void *tmp = malloc( size );
    if ( tmp == NULL ) {
        perror( errmsg );
        exit( EXIT_FAILURE );
    }
    return tmp;
}

int int64_t_cmp(const void * a_, const void * b_) {
    const int64_t * a = a_;
    const int64_t * b = b_;
    return strcmp(S + *a, S + *b);
}

int main( int argc, char *argv[] ) {

    if ( argc != 4 ) {
        printf( "usage: %s infile safile n\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }
    
    FILE *rfp = fopen( argv[ 1 ], "r" );
    if ( rfp == NULL ) { fprintf( stderr, "%s doesn't exist\n", argv[1] ); exit( EXIT_FAILURE );}
    FILE *safp = fopen( argv[ 2 ], "w" );
    if ( safp == NULL ) { fprintf( stderr, "%s doesn't exist\n", argv[2] ); exit( EXIT_FAILURE );}
    int n = atoi( argv[ 3 ] );
    /* check file size */
    fseek( rfp, 0, SEEK_END );
    size_t fsize = ftell( rfp );
    fseek( rfp, 0, SEEK_SET );
    if ( fsize < ( size_t ) n ) {
        fprintf( stderr, "N is too big\n" );
        exit( EXIT_FAILURE );
    }
    
    int i;
    int64_t * sa = mymalloc(sizeof(int64_t) * n, "sa");
    S = ( char * ) mymalloc( sizeof( char ) * (n + 1), "S" );

    fread( S, sizeof( char ), n, rfp );
    S[ n ] = '\0';

    for (i = 0; i < n; i++) {
        sa[i] = i;
    }
    
    qsort(sa, n, sizeof( int64_t ), int64_t_cmp);

    fwrite( sa, sizeof( int64_t ), n, safp );

    
    fclose( rfp );
    fclose( safp );
    free( sa );
    free( S );
    
    return 0;
}
