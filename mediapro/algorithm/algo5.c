#include <stdio.h>
#include <stdlib.h>

void make_sa(char* S, long* sa, long n);

int compare_sa(long i, long j);

typedef long value_type;

value_type med3(value_type x, value_type y, value_type z) {
    if (x < y) {
        if (y < z) return y; else if (z < x) return x; else return z;
    } else {
        if (z < y) return y; else if (x < z) return x; else return z;
    }
}
//クイックソート
void quicksort(value_type a[], int left, int right) {
    if (left < right) {
        int i = left, j = right;
        value_type tmp, pivot = med3(a[i], a[i + (j - i) / 2], a[j]); /* (i+j)/2 ではオーバーフローしてしまう */
        while (1) { /* a[] を pivot 以上と以下の集まりに分割する */
            while (a[i] < pivot) i++; /* a[i] >= pivot となる位置を検索 */
            while (pivot < a[j]) j--; /* a[j] <= pivot となる位置を検索 */
            if (i >= j) break;
            tmp = a[i]; a[i] = a[j]; a[j] = tmp; /* a[i], a[j] を交換 */
            i++; j--;
        }
        quicksort(a, left, i - 1);  /* 分割した左を再帰的にソート */
        quicksort(a, j + 1, right); /* 分割した右を再帰的にソート */
    }
}

int sign(long n){
    return n > 0 ? 1:-1;
}
long n, k;
long* rank;
long* tmp;
long* sa;

int main(int argc, char** argv){
    FILE* fp = fopen(argv[1], "r");
    char c;
    n = 0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' ')
        n++;
    }
    char* S = (char*)malloc(sizeof(char)*n);
    fseek(fp, SEEK_SET, 0);
    printf("Ok");
    n =0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' '){
            S[n] = c;
            n++;
        }
    }
    sa = (long*)malloc(sizeof(long)*(n+1));
    printf("OK");
    make_sa(S, sa, n);
    return 0;
}

int compare_sa(long i, long j){
    if(rank[i] != rank[j]){
        return sign(rank[i] < rank[j]);
    }
    else{
        long ri = i + k <= n ? rank[i+k] : -1;
        long rj = j + k <= n ? rank[j+k] : -1;
        return sign(ri < rj);
    }
}

void make_sa(char* S, long* sa, long n){
    for(long i=0; i<=n; i++){
        sa[i] = i;
        rank[i] = i<n ? S[i] : -1;
    }

    for(long i=1; i<= n; i*= 2){
        quicksort(sa, 0, n);
        tmp[sa[0]] = 0;
        for(long j =0; j<= n; j++){
            tmp[sa[j]] = tmp[sa[j-1]] + (compare_sa(sa[j-1], sa[j])? 1: 0);
        }
        for(long j = 0;j<n;j++){
            rank[j] = tmp[j];
        }
    }

}