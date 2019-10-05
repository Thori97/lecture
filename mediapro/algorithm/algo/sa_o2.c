//! gcc -o sa_o2.bin sa_o2.c -W -Wall -std=gnu99 -O3
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

void *mymalloc( size_t size, char *errmsg )
{
    void *tmp = malloc( size );
    if ( tmp == NULL ) {
        perror( errmsg );
        exit( EXIT_FAILURE );
    }
    return tmp;
}

int int_cmp(const void * a_, const void * b_) {
    const int64_t * a = a_;
    const int64_t * b = b_;
    return strcmp(S + *a, S + *b);
}

int int_bsearch_ex( char *k, int64_t *a, int n )
{
    int l = 0, r = n-1;
    while ( r - l > 1 ) {
        int mid = ( r + l ) / 2;
        int cmp = strcmp( k, S + a[ mid ] );

        if ( cmp > 0 ) {
            l = mid;
            continue;
        } else if ( cmp < 0 ) {
            r = mid;
            continue;
        } else if ( cmp == 0 ) {
            return mid;
        }
    }
    if ( strcmp( k, S + a[ l ] ) <= 0 ) {
        return l;
    }
    if ( strcmp( k, S + a[ r ] ) > 0 ) {
        return r+1;
    }
    return r;
}


int main( int argc, char *argv[] )
{
    if ( argc != 4 ) {
        printf( "usage: %s infile safile search\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }
    
    FILE *rfp = fopen( argv[ 1 ], "r" );
    if ( rfp == NULL ) { fprintf( stderr, "%s doesn't exist\n", argv[1] ); exit( EXIT_FAILURE );}
    FILE *safp = fopen( argv[ 2 ], "r" );
    if ( safp == NULL ) { fprintf( stderr, "%s doesn't exist\n", argv[2] ); exit( EXIT_FAILURE );}
    char *key = argv[ 3 ];
    /* check file size */
    /* fseek( rfp, 0, SEEK_END ); */
    /* size_t rsize = ftell( rfp ); */
    fseek( safp, 0, SEEK_END );
    size_t sasize = ftell( safp );
    int n = sasize / sizeof( int64_t );
    fseek( safp, 0, SEEK_SET );
    
    /* read sa file */
    int64_t * sa = mymalloc( sizeof( int64_t ) * n, "sa" );
    int ret = fread( sa, sizeof( int64_t ), n, safp );
    if ( ret < n ) {
        n = sasize / sizeof( int64_t );
    }
    /* read string file */
    S = ( char * ) mymalloc( sizeof( char ) * ( n + 1 ), "S" );
    fread( S, sizeof( char ), n, rfp );
    S[ n ] = '\0';
    
    ret = int_bsearch_ex( key, sa, n );
    if ( ret >= n ) return 0;
    while ( strncmp( key, S + sa[ ret ], strlen( key ) ) == 0 ) {
        printf( "%lld\n", sa[ ret ] );
        ret++;
    }
    
    
    fclose( rfp );
    fclose( safp );
    free( sa );
    free( S );
    
    return 0;
}
