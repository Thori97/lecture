//!gcc -o sa3.bin sa3.c -W -Wall -O3 -std=gnu99

/*
 * to get filesize, type the below command
 *
 * LANG=C ls -l | grep kaki.txt | sed -E 's/ +/ /g' | cut -f 5 -d ' '
 *
 */


/* -*-coding: utf-8 -*-
 * sa3.c
 * 
 * if you can't read the following, please consult me.
 * この文字が無事読めたら良し
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* int string_cmp(const void * a_, const void * b_) { */
/*     const sa_t const * a = a_; */
/*     const sa_t const * b = b_; */
/*     return strcmp( a->str, b->str ); */
/* } */

int string_cmp(const void * a_, const void * b_) {
    const char * const * a = a_;
    const char * const * b = b_;
    return strcmp( *a, *b );
}

int bsearch_ex( char *k, char **a, int n )
{
    int l = 0, r = n-1;
    while ( r - l > 1 ) {
        int mid = ( r + l ) / 2;
        int cmp = strcmp( k, a[ mid ] );
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
    if ( strcmp( k, a[ l ] ) <= 0 ) return l;
    if ( strcmp( k, a[ r ] ) > 0 ) return r+1;
    return r;
}

void *mymalloc( size_t size, char *errmsg )
{
    void *tmp = malloc( size );
    if ( tmp == NULL ) {
        perror( errmsg );
        exit( EXIT_FAILURE );
    }
    return tmp;
}

int main( int argc, char *argv[] ) {

    if ( argc != 4 ) {
        printf( "usage: %s infile search insize\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }

    /* initialize variables */
    FILE *fp = fopen( argv[ 1 ], "r" );
    char *key = argv[ 2 ];      /* search key */
    int n = atoi( argv[ 3 ] );  /* filesize */
    int i;
    char **a = ( char ** ) mymalloc( sizeof( char * ) * n, "sa" );
    char *s = ( char * ) mymalloc( sizeof( char ) * ( n + 1 ), "s" );

    /* check file size */
    /* fseek( fp, 0, SEEK_END ); */
    /* size_t fsize = ftell( fp ); */
    /* printf( "%s is opened.\n" */
    /*         "size: %8d Bytes\n", */
    /*         argv[ 1 ], fsize ); */

    /* read file */
    fseek( fp, 0, SEEK_SET );
    int retsize = fread( s, sizeof( char ), n, fp );
    if ( retsize != n ) n = retsize; /* real filesize < N */
    s[ n ] = '\0';

    /* create SA */
    for (i = 0; i < n; i++) {
        a[ i ] = s + i;
    }
    qsort(a, n, sizeof( char * ), string_cmp);

    int ret = bsearch_ex( key, a, n );
    if ( ret >= n ) return 0;
    while ( strncmp( key, a[ ret ], strlen( key ) ) == 0 ) {
        printf( "%d\n", n - strlen( a[ ret ] ) );
        ret++;
    }

    fclose( fp );
    free( a );
    free( s );

    return 0;
}
