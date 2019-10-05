/* 
 * ex1_10.c
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>

/* ex1: doubleの配列をソート */

int double_cmp(const void * a_, const void * b_) {
  const double * a = a_, * b = b_;
  return *a - *b;
}

void ex1() {
  double a[3] = { 3.1, 1.2, 2.1 };
  qsort(a, 3, sizeof(double), double_cmp);
  printf("%f %f %f\n", a[0], a[1], a[2]);
  assert(a[0] < a[1]);
  assert(a[1] < a[2]);
  printf("ex1 OK\n");
}

/* ex2: point構造体の配列をソート */

typedef struct point { double x; double y; } point;
int point_cmp(const void * a_, const void * b_) {
  const point * a = a_, * b = b_;
  double da = a->x*a->x + a->y*a->y;
  double db = b->x*b->x + b->y*b->y;
  return da - db;
}

void ex2() {
  point p[3] = { { 3.0, 4.0 }, { 5.0, 6.0 }, { 1.0, 2.0 } };
  qsort(p, 3, sizeof(point), point_cmp);
  printf("(%f,%f) (%f,%f) (%f,%f)\n", 
	 p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y);
  assert(point_cmp(p,   p+1) < 0);
  assert(point_cmp(p+1, p+2) < 0);
  printf("ex2 OK\n");
}

/* ex3: 文字列の配列をソート */
int string_cmp(const void * a_, const void * b_) {
  const char * const * a = a_, * const * b = b_;
  return strcmp(*a, *b);
}

void ex3() {
  char * s[4] = { "mimura", "matsumoto", "otake", "hamada" };
  qsort(s, 4, sizeof(char*), string_cmp);
  printf("%s %s %s %s\n", s[0], s[1], s[2], s[3]);
  assert(strcmp(s[0], s[1]) < 0);
  assert(strcmp(s[1], s[2]) < 0);
  assert(strcmp(s[2], s[3]) < 0);
  printf("ex3 OK\n");
}

/* ex4: doubleの配列を二分探索 */
void ex4() {
  double a[3] = { 3.1, 1.2, 2.1 };
  double k = 1.2;
  double * m = bsearch(&k, a, 3, sizeof(double), double_cmp);
  assert(m == &a[1]);
  printf("ex4 OK\n");
}

/* ex5: 鍵,値のペアの配列を二分探索
   --> 連想配列 */
typedef char * K;
typedef double V;
typedef struct KV { K k; V v; } KV;
int kv_cmp(const void * a_, const void * b_) {
  const KV * a = a_, * b = b_;
  return strcmp(a->k, b->k);
}
V kv_find(KV * kvs, int n, K k, V not_found) {
  KV * f = bsearch(&k, kvs, n, sizeof(KV), kv_cmp);
  if (f) return f->v;
  return not_found;
}
void ex5() {
  KV kvs[4] = { { "mimura", 1.0 }, { "matsumoto", 2.0 }, 
		{ "otake", 3.0 }, { "hamada", 4.0 } };
  double x = kv_find(kvs, 4, "otake", -1.0);
  assert(x == 3.0);
  printf("ex5 OK\n");
}

/* ex6: 文字列の配列を二分探索 */
void ex6() {
  char * s[4] = { "mimura", "matsumoto", "otake", "hamada" };
  char * k = "otake";
  char ** m = bsearch(&k, s, 4, sizeof(char*), string_cmp);
  assert(m == &s[2]);
  printf("ex6 OK\n");
}

/* ex7: point構造体へのポインタを格納する平衡探索木 */
void ex7() {
  void * root = NULL;
  point a = { 1.1, 2.2 }, b = { 3.3, 4.4 };
  point ** p = tsearch(&a, &root, point_cmp);
  assert(*p == &a);
  point ** q = tfind(&a, &root, point_cmp);
  assert(*q == &a);
  point ** r = tfind(&b, &root, double_cmp);
  assert(!r);
  printf("ex8 OK\n");
}

/* ex8: intを格納する平衡探索木
   intをそのままポインタ変数に格納 */
int int_cmp(const void * a_, const void * b_) {
  long a = (long)a_, b = (long)b_;
  return a - b;
}

void ex8() {
  void * root = NULL;
  int * p = tsearch((void*)100, &root, int_cmp);
  assert(*p == 100);
  int * q = tfind((void*)100, &root, int_cmp);
  assert(*q == 100);
  int * r = tfind((void*)200, &root, int_cmp);
  assert(!r);
  printf("ex8 OK\n");
}

/* ex9: 文字列(char*)を格納する平衡探索木 */
int string_cmp2(const void * a_, const void * b_) {
  const char * a = a_, * b = b_;
  return strcmp(a, b);
}

void ex9() {
  void * root = NULL;
  char ** p = tsearch("mimura", &root, string_cmp2);
  assert(strcmp(*p, "mimura") == 0);
  char ** q = tfind("mimura", &root, string_cmp2);
  assert(strcmp(*q, "mimura") == 0);
  char ** r = tfind("otake", &root, string_cmp2);
  assert(!r);
  printf("ex9 OK\n");
}

/* ex10: 鍵,値のペアへのポインタを格納する平衡探索木 */
void ex10() {
  void * root = NULL;
  KV a = { "mimura", 1.1 }, b = { "otake", 2.2 };
  KV ** p = tsearch(&a, &root, kv_cmp);
  assert(*p == &a);
  KV ** q = tfind(&a, &root, kv_cmp);
  assert(*q == &a);
  KV ** r = tfind(&b, &root, kv_cmp);
  assert(!r);
  printf("ex10 OK\n");
}

int main() {
  ex1();
  ex2();
  ex3();
  ex4();
  ex5();
  ex6();
  ex7();
  ex8();
  ex9();
  ex10();
  return 0;
}
