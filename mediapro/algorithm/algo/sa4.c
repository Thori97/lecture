//!gcc -o sa4.bin sa4.c -W -Wall -O3 -std=gnu99

/*
 * to get filesize, type the below command
 *
 * LANG=C ls -l | grep kaki.txt | sed -E 's/ +/ /g' | cut -f 5 -d ' '
 *
 */


/* -*-coding: utf-8 -*-
 * sa4.c
 * 
 * if you can't read the following, please consult me.
 * この文字が無事読めたら良し
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>

uint64_t count_strcmp = 0;

int string_cmp(const void * a_, const void * b_) {
    const char * const * a = a_;
    const char * const * b = b_;
    count_strcmp++;
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
    if ( strcmp( k, a[ l ] ) <= 0 ) {
        return l;
    }
    if ( strcmp( k, a[ r ] ) > 0 ) {
        return r+1;
    }
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

double get_current_ms( void )
{
    struct timeval tv;
    gettimeofday( &tv, NULL );
    return tv.tv_sec*1E3 + ( double ) tv.tv_usec / 1E3;
}

int main( int argc, char *argv[] ) {

    if ( argc != 5 ) {
        printf( "msage: %s infile insize length num\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }

    /* initialize variables */
    FILE *fp = fopen( argv[ 1 ], "r" );
    int n = atoi( argv[ 2 ] );  /* filesize */
    int l = atoi( argv[ 3 ] );  /* cut length */
    int k = atoi( argv[ 4 ] );  /* # search elements */
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
    double read_before = get_current_ms();
    int retsize = fread( s, sizeof( char ), n, fp );
    double read_after = get_current_ms();
    if ( retsize != n ) n = retsize; /* real filesize < N */
    s[ n ] = '\0';

    /* create SA */
    double sa_before = get_current_ms();
    for (i = 0; i < n; i++) {
        a[ i ] = s + i;
    }
    qsort(a, n, sizeof( char * ), string_cmp);
    double sa_after = get_current_ms();

    double search_before = get_current_ms();
    char key[ l + 1 ];          /* C99 */
    for ( i = 0; i < k; i++ ) {
        if ( i * ( l + 1 ) <= n ) {
            memcpy( key, s + ( i * l ), l );
        } else {
            memset( key, 0, l + 1 );
            memcpy( key, s + ( i * l ), n - i * ( l + 1 ) );
        }

        int ret = bsearch_ex( key, a, n );
        if ( ret >= n ) return 0;
        while ( strncmp( key, a[ ret ], strlen( key ) ) == 0 ) {
            /* printf( "%d\n", n - strlen( a[ ret ] ) ); */
            ret++;
        }
    }
    double search_after = get_current_ms();

    printf( "%8.3f %12llu %8.3f %8.3f\n", ( read_after-read_before ), count_strcmp, ( sa_after - sa_before ), ( search_after - search_before ) );
    fclose( fp );
    free( a );
    free( s );

    return 0;
}
