//!gcc -o sa1.bin sa1.c -W -Wall -O3 -std=gnu99
/* -*-coding: utf-8 -*-
 * sa1.c
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

int string_cmp(const void * a_, const void * b_) {
    const sa_t const * a = a_;
    const sa_t const * b = b_;
    return strcmp( a->str, b->str );
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
    sa_t *a = ( sa_t * ) malloc( sizeof( sa_t ) * n );
    if ( a == NULL ) {
        perror( "malloc" );
        exit( EXIT_FAILURE );
    }
    
    for (i = 0; i < n; i++) {
        a[ i ].id = i;
        a[ i ].str = s + i;
    }
    qsort(a, n, sizeof( sa_t ), string_cmp);

    sa_t key = {
        .str = argv[ 1 ]
    };
    sa_t *res = bsearch( &key, a, n, sizeof( sa_t ), string_cmp );

    printf( "%d\n", ( res ) ? res->id : -1 );

    return 0;
}
