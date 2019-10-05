//!gcc -o sa2.bin sa2.c -W -Wall -O3 -std=gnu99
/* -*-coding: utf-8 -*-
 * sa2.c
 * 
 * if you can't read the following, please consult me.
 * この文字が無事読めたら良し
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sa
{
    int id;
    char *str;
} sa_t;

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

int main( int argc, char *argv[] ) {
    if ( argc != 2 ) {
        printf( "usage: %s string\n", argv[ 0 ] );
        exit( EXIT_FAILURE );
    }
    
    char * s = "となりのきゃくはよくかきくうきゃくだ";
    //char * s = "world cup";
    int n = strlen(s);
    int i;
    /* sa_t *a = ( sa_t * ) malloc( sizeof( sa_t ) * n ); */
    char **a = ( char ** ) malloc( sizeof( char * ) * n );
    if ( a == NULL ) {
        perror( "malloc" );
        exit( EXIT_FAILURE );
    }
    
    for (i = 0; i < n; i++) {
        /* a[ i ].id = i; */
        /* a[ i ].str = s + i; */
        a[ i ] = s + i;
    }
    qsort(a, n, sizeof( char * ), string_cmp);

    char *key = argv[ 1 ];
    int ret = bsearch_ex( key, a, n );
    if ( ret >= n ) return 0;
    while ( strncmp( key, a[ ret ], strlen( key ) ) == 0 ) {
        printf( "%d\n", n - strlen( a[ ret ] ) );
        ret++;
    }

    return 0;
}
