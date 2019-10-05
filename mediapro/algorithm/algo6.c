#include <stdio.h>
#include <stdlib.h>

long n, k;
long* rank;
long* sa;
long* tmp;

void make_sa(char* S);

_Bool compare_sa(long i, long j);

void quicksort(long* sa, long left, long right);

void reverse(long* sa);

long med3(long x, long y, long z) {
    if (x < y) {
        if (y < z) return y; else if (z < x) return x; else return z;
    } else {
        if (z < y) return y; else if (x < z) return x; else return z;
    }
}

void swap(long *x,long *y)
{
    long temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
long choose_pivot(long i,long j )
{
    return((i+j) /2);
}

void MergeSort(long x[ ], long left, long right)
{
    long mid, i, j, k;
	long temp[right+1];
    if (left >= right)              /* 配列の要素がひとつなら */
        return;                     /* 何もしないで戻る */

                                    /* ここでは分割しているだけ */
    mid = (left + right) / 2;       /* 中央の値より */
    MergeSort(x, left, mid);        /* 左を再帰呼び出し */
    MergeSort(x, mid + 1, right);   /* 右を再帰呼び出し */

      /* x[left] から x[mid] を作業領域にコピー */
    for (i = left; i <= mid; i++)
        temp[i] = x[i];

      /* x[mid + 1] から x[right] は逆順にコピー */
    for (i = mid + 1, j = right; i <= right; i++, j--)
        temp[i] = x[j];

    i = left;         /* i とj は作業領域のデーターを */
    j = right;        /* k は配列の要素を指している */

    for (k = left; k <= right; k++)    /* 小さい方から配列に戻す */
        if (compare_sa(temp[i],temp[j])==0)        /* ここでソートされる */
            x[k] = temp[i++];
        else
            x[k] = temp[j--];
}


int main(int argc, char** argv){
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("file dame\n");
    }
    else{
        printf("fileOK\n");
    }
    char c;
    n = 0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' ')
        n++;
    }
    char* S = (char*)malloc(sizeof(char)*(n+1));
    if(S == NULL){
        printf("SNG\n");
    }
    else{
        printf("SOK\n");
    }
    fseek(fp, SEEK_SET, 0);
    n =0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' '){
            S[n] = c;
            n++;
        }
    }
    S[n] = 0;//space抜きの配列Sができた
    sa = (long*)malloc(sizeof(long)*(n+1));
    if(sa == NULL){
        printf("saNG\n");
    }
    else{
        printf("saOK\n");
    }
    rank = (long*)malloc(sizeof(long)*(n+1));
    if(rank == NULL){
        printf("rankNG\n");
    }
    else{
        printf("rankOK\n");
    }
    tmp = (long*)malloc(sizeof(long)*(n+1));
    if(tmp == NULL){
        printf("tmpNG\n");
    }
    printf("\n");
    make_sa(S);
    for(long i=0;i<n+1;i++){
        printf("%ld ",sa[i]);
    }
    free(S);
    free(sa);
    free(rank);
    free(tmp);
    return 0;
}

void make_sa(char* S){
    for(long i=0;i<n+1;i++){
        sa[i] = i;
        rank[i] = i<n ? S[i] : -1;
    }
    printf("rank= ");
        for(long i=0;i<n+1;i++) printf("%ld ", rank[i]);
        printf("\n");
    for(long k=1;k<=n;k*=2){
        MergeSort(sa, 0, n);
        reverse(sa);
        printf("k=%ld   ", k);
        for(long i=0;i<n+1;i++) printf("%ld ", sa[i]);
        printf("\n");
        
        tmp[sa[0]] = 0;
        for(long i=1;i<=n;i++){
            tmp[sa[i]] = tmp[sa[i-1]]+(compare_sa(sa[i-1], sa[i]) ? 1:0);
        }
        for(long i=0;i<=n;i++){
            rank[i] = tmp[i];
        }
        printf("rank= ");
        for(long i=0;i<n+1;i++) printf("%ld ", rank[i]);
        printf("\n");
    }
}

_Bool compare_sa(long i, long j){
    if(rank[i] != rank[j]){
        return rank[j] > rank[i];
    }
    else{
       long ri = (i+k<=n ? rank[i+k] : -1);
       long rj = (j+k<=n ? rank[j+k] : -1);
       return ri < rj;
    }
}

void quicksort(long *list,long m,long n)
{
    long key,i,j,k;
    if( m < n)
    {
        k = choose_pivot(m,n);
        swap(&list[m],&list[k]);
        key = list[m];
        i = m+1;
        j = n;
        while(i <= j)
        {
            while((i <= n) && (compare_sa(list[i],key)==0))
                i++;
            while((j >= m) && (compare_sa(list[j],key)==1))
                j--;
            if( i < j)
                swap(&list[i],&list[j]);
        }
        /* swap two elements */
        swap(&list[m],&list[j]);
 
        /* recursively sort the lesser list */
        quicksort(list,m,j-1);
        quicksort(list,j+1,n);
    }
}

void reverse(long* sa){
    long tmp1[n+1];
    for(long i=0;i<n+1;i++) tmp1[i] = sa[n-i];
    for(long i=0;i<n+1;i++) sa[i] = tmp1[i];
}