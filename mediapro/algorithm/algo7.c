#include <stdio.h>
#include <stdlib.h>

long n, k;
long *rank;
long *sa;
long *tmp;
char *t;
char *S;
long *bin;

long* induced_sort(long* lmss, long lmsc);

short lms(char *t, long i);

void quicksort(long *sa, long left, long right);

long* SAIS(char *S, int k);

int main(int argc, char **argv){
    FILE* fp = fopen(argv[1], "r");
    if(fp != NULL){
        printf("file OK\n");
    }
    char c;
    n = 0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' ')
        n++;
    }
    S = (char*)malloc(sizeof(char)*(n+1));
    if(S != NULL){
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
    if(sa != NULL) printf("saOK\n");
    rank = (long*)malloc(sizeof(long)*(n+1));
    if(rank != NULL) printf("rankOK\n");
    tmp = (long*)malloc(sizeof(long)*(n+1));
    if(tmp != NULL) printf("tmpOK\n"); 
    t = (char*)malloc(sizeof(char)*(n+1));
    if(t != NULL) printf("tOK\n");
    bin = (long*)malloc(sizeof(char)*256);
    for(int i=0;i<256;i++) bin[i] = 0;
    sa = SAIS(S, 256);
    for(long i=0;i<n+1;i++) printf("%c ",t[i]);
    printf("\n");
    for(long i=0;i<n+1;i++) printf("%ld ", sa[i]);
    printf("\n");

    free(S);
    free(sa);
    free(rank);
    free(tmp);
    return 0;
}

long* induced_sort(long* lmss, long lmsc){
    for(long i=0;i<256;i++) bin[i] = 0;
    for(long i=0;i<n+1;i++) bin[S[i]+1]++;
    long sum =0;
    for(long i=0;i<256;i++) bin[i] = (sum+=bin[i]); 
    
    for(long i=0;i<n+1;i++) sa[i] = -1;
    //step1
    long count[256];
    long ch;
    for(int i=0;i<256;i++) count[i] = 0;
    for(long i=0;i<lmsc;i++){
        long j = lmss[lmsc-i-1];
        ch = S[j];
        sa[bin[ch+1] -1 -count[ch]] = j;
        count[ch] ++;
    }
    //step2
    for(int i=0;i<256;i++) count[i] = 0;
    for(long i=0;i<n+1;i++){
        if(sa[i]==-1 || sa[i]==0 || t[sa[i]-1] =='S'){

        }
        else{
            ch = S[sa[i]-1];
            sa[bin[ch]+count[ch]] = sa[i]-1;
            count[ch] ++;
        }
    }
    //step3
    for(int i=0;i<256;i++) count[i] = 0;
    for(long i=n;i>-1;i--){
        if(sa[i]==-1 || sa[i] == 0||t[sa[i]-1]=='L'){

        }
        else{
            ch = S[sa[i]-1];
            sa[bin[ch+1]-1-count[ch]] = sa[i]-1;
            count[ch] ++;
        }
    }
    printf("\n");
    for(long i=0;i<n+1;i++) printf("%ld ", sa[i]);
    printf("\n");
    return sa;
}

short lms(char *t, long i){
    if(i>0 && t[i-1]=='L' && t[i] == 'S') return 1;
    else return 0;
}

void quicksort(long* numbers, long left, long right)
{
    long pivot, l_hold, r_hold;

    l_hold = left;
    r_hold = right;
    pivot = numbers[left];
    while (left < right)
    {
        while ((numbers[right] >= pivot) && (left < right))
            right--;
        if (left != right)
        {
            numbers[left] = numbers[right];
            left++;
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++;
        if (left != right)
        {
            numbers[right] = numbers[left];
            right--;
        }
    }
    numbers[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot)
        quicksort(numbers, left, pivot-1);
    if (right > pivot)
        quicksort(numbers, pivot+1, right);
}

long* SAIS(char *S, int k){
    t[n] = 'S';
    for(long i=n-1;i>-1;i--){
        if(S[i]<S[i+1]) t[i] = 'S';
        else if(S[i]>S[i+1]) t[i] = 'L';
        else t[i] = t[i+1];
    }

    long lmsc=0, c1 = 0;
    for(long i=0;i<n+1;i++) lmsc += lms(t, i);
    long lmss[lmsc];
    for(long i=0;i<n+1;i++){
        if(lms(t, i)){
            lmss[c1]  = i;
            c1 ++;
        }
    }

    long *seed = lmss;
    sa = induced_sort(seed, lmsc);

    lmsc=0, c1 = 0;
    for(long i=0;i<n+1;i++) lmsc += lms(t, sa[i]);
    lmss[lmsc];
    for(long i=0;i<n+1;i++){
        if(lms(t, sa[i])){
            lmss[c1]  = sa[i];
            c1 ++;
        }
    }


    long nums[n+1];//??????
    for(long i=0;i<n+1;i++) nums[i] = -1;
    long num;
    nums[lmss[0]] = num = 0;//daijoubu?

    for(long i=0; i<lmsc-1;i++){
        short diff = 0;
        long d =0;
        for(long d=0;d<n+1;d++){
            if(S[lmss[i]+d] != S[lmss[i+1]+d] || lms(t, lmss[i]+d) != lms(t, lmss[i+1]+d)){
                diff = 1;
                break;
            }
            else if(d>0 && (lms(t, lmss[i]+d)>0||lms(t, lmss[i+1]+d))){
                break;
            }
        }
        if(diff==1) num ++;
        nums[lmss[i+1]] = num;
    }


    long c2=0;
    for(long i=0;i<n+1;i++){if(nums[i]!=-1){c2++;}}
    long nums2[c2]; c2 =0;
    for(long i=0;i<n+1;i++){if(nums[i]!=-1){nums2[c2] = nums[i];c2++;}}


    long sa2[c2];
    if(num+1<c2){long *sa2 = SAIS(nums2, num+1);}
    else{
        for(long i=0;i<c2;i++) sa2[nums2[i]] = i;
    }

    for(long i=0;i<c2;i++) {sa2[i] = lmss[sa2[i]];}
    printf("new seed = ");
    for(long i=0;i<c2;i++) printf("%ld ", sa2[i]);
    long *sa3 = induced_sort(sa2, c2);
    return sa3;
}