//!gcc -o sa_example.bin sa_example.c -W -Wall -O3 -std=gnu99
/* -*-coding: utf-8 -*-
 * sa_example.c
 * 
 * if you can't read the following, please consult me.
 * この文字が無事読めたら良し
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int string_cmp(const void * a_, const void * b_) {
    const char * const * a = a_;
    const char * const * b = b_;
    return strcmp(*a, *b);
}

int main( int argc, char *argv[] ) {
    
    char * s = "となりのきゃくはよくかきくうきゃくだ";
    //char * s = "world cup";
    int n = strlen(s);
    int i;
    char ** a = malloc(sizeof(char*) * n);
    for (i = 0; i < n; i++) {
        a[i] = s + i;
    }
    qsort(a, n, sizeof(char*), string_cmp);
    for (i = 0; i < n; i++) {
        printf("%s\n", a[i]);
    }
    return 0;
}
