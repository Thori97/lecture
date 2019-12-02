#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

/* *p を不可分に +1 する関数を 
   __sync_bool_compare_and_swap
   を使って実装 */
void inc_cas(volatile long * p) {
  /* この中を埋めよ */
   while(!__sync_bool_compare_and_swap (p, *p, (*p)+1)){
       ;
   }
}
int main() {
  volatile long x = 0;
#pragma omp parallel num_threads(2)
  {
    for (long i = 0; i < 10000000; i++) {
      inc_cas(&x);
    }
  }
  printf("x = %ld\n", x); fflush(stdout);
  assert(x == 20000000);
  printf("OK\n");
  return 0;
}

