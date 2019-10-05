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

int int_cmp(const void * a_, const void * b_) {
  const int * a = a_;
  const int * b = b_;
  return strcmp(S + *a, S + *b);
}

int main() {
  S = "となりのきゃくはよくかきくうきゃくだ";
  int n = strlen(S);
  int i;
  int * sa = malloc(sizeof(int) * n);
  for (i = 0; i < n; i++) {
    sa[i] = i;
  }
  qsort(sa, n, sizeof(int), int_cmp);
  for (i = 0; i < n; i++) {
    printf("%3d : %s\n", sa[i], S + sa[i]);
  }
  return 0;
}
