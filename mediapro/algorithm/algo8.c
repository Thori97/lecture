#include <stdio.h>
#include <stdlib.h>

char mask[]={0x80, 0x40, 0x20, 0x10, 0x08,0x04, 0x02, 0x01};
#define tget(i) ( (t[(i)/8]&mask[(i)%8]) ? 1 : 0 )
#define tset(i, b) t[(i)/8]=(b)?(mask[(i)%8]|t[(i)/8]):(( ~mask[(i)%8])&t[(i)/8])
#define chr(i) (cs==sizeof(long)?((long*)s)[i]:((char *)s)[i])
#define isLMS(i) (i>0 && tget(i) && !tget(i-1))
// 開始or終了を見つける
void getBuckets(char *s, long *bkt, long n,
long K, long cs,_Bool end) {
long i, sum=0;
// Bucketをclearする
for(i=0; i<=K; i++) bkt[i]=0;
// それぞれのbucketを計算
for(i=0; i<n; i++) bkt[chr(i)]++;
for(i=0; i<=K; i++)
{ sum+=bkt[i]; bkt[i]=end ? sum : sum-bkt[i]; }
}
// SAlを計算
void induceSAl(char *t, long *SA,
char *s, long *bkt,
long n, long K, long cs, _Bool end) {
long i, j;
// bucketsの開始位置
getBuckets(s, bkt, n, K, cs, end);
for(i=0; i<n; i++) {
j=SA[i]-1;
if(j>=0 && !tget(j)) SA[bkt[chr(j)]++]=j;
}
}
// SAsを計算
void induceSAs(char *t, long *SA,
char *s, long *bkt,
long n, long K, long cs, _Bool end) {
long i, j;
// bucketsの終了位置
getBuckets(s, bkt, n, K, cs, end);

for(i=n-1; i>=0; i--) {
j=SA[i]-1;
if(j>=0 && tget(j)) SA[--bkt[chr(j)]]=j;
}
}
// bucketの配列

// suffix array SA (s[0..n-1] in {1..K}ˆn)を特定
// s[n-1]=0 , n>=2
void SA_IS(char *s, long *SA, long n,
long K, long cs) {
// LS-typeを入れる
char *t=(char *)malloc(n/8+1);
long i, j;
// 文字を特定
tset(n-2, 0); tset(n-1, 1);
for(i=n-3; i>=0; i--)
tset(i, (chr(i)<chr(i+1)
|| (chr(i)==chr(i+1)
&& tget(i+1)==1))?1:0);
// stage 1:　計算量半分に
// sort all the S-substrings
// bucket array
long *bkt = (long *)malloc(sizeof(long)*(K+1));
// bucketsの終了位置
getBuckets(s, bkt, n, K, cs, 1);
for(i=0; i<n; i++) SA[i]=-1;
for(i=1; i<n; i++)
if(isLMS(i)) SA[--bkt[chr(i)]]=i;
induceSAl(t, SA, s, bkt, n, K, cs, 0);
induceSAs(t, SA, s, bkt, n, K, cs, 1);
free(bkt);
// compact all the sorted substrings into
// the first n1 items of SA
// 2*n1 must be not larger than n (proveable)
long n1=0;
for(i=0; i<n; i++)
if(isLMS(SA[i])) SA[n1++]=SA[i];
// find the lexicographic names of substrings
// init the name array buffer
for(i=n1; i<n; i++) SA[i]=-1;
long name=0, prev=-1;
for(i=0; i<n1; i++) {
long pos=SA[i]; _Bool diff=0;
for(long d=0; d<n; d++)
if(prev==-1 || chr(pos+d)!=chr(prev+d)
|| tget(pos+d)!=tget(prev+d))
{ diff=1; break; }
else if(d>0 && (isLMS(pos+d) ||
isLMS(prev+d)))
break;
if(diff) { name++; prev=pos; }
pos=(pos%2==0)?pos/2:(pos-1)/2;
SA[n1+pos]=name-1;
}
for(i=n-1, j=n-1; i>=n1; i--)
if(SA[i]>=0) SA[j--]=SA[i];
// stage 2: solve the reduced problem
// recurse if names are not yet unique
long *SA1=SA, *s1=SA+n-n1;
if(name<n1)
SA_IS((char*)s1, SA1, n1,
name-1, sizeof(long));
else // generate the suffix array of s1 directly
for(i=0; i<n1; i++) SA1[s1[i]] = i;
// stage 3: induce the result for
// the original problem
// bucket array
bkt = (long *)malloc(sizeof(long)*(K+1));
// put all the LMS characters into their buckets
// find ends of buckets
getBuckets(s, bkt, n, K, cs, 1);
for(i=1, j=0; i<n; i++)
if(isLMS(i)) s1[j++]=i; // get p1
// get index in s
for(i=0; i<n1; i++) SA1[i]=s1[SA1[i]];
// init SA[n1..n-1]
for(i=n1; i<n; i++) SA[i]=-1;
for(i=n1-1; i>=0; i--) {
j=SA[i]; SA[i]=-1;
SA[--bkt[chr(j)]]=j;
}
induceSAl(t, SA, s, bkt, n, K, cs, 0);
induceSAs(t, SA, s, bkt, n, K, cs, 1);
free(bkt); free(t);
}
void makelcp(char* S, long *sa, long *lcp, long n);
long *rank;
int main(int argc, char** argv){
    FILE* fp = fopen(argv[1], "r");
    char c;
    long n = 0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' ')
        n++;
    }
    char* S = (char*)malloc(sizeof(char)*(n+1));
    fseek(fp, SEEK_SET, 0);
    n =0;
    while( (c = fgetc(fp)) != EOF ){
        if(c != ' '){
            S[n] = c;
            n++;
        }
    }
    S[n] = 0;//space抜きの配列Sができた
    long* sa = (long*)malloc(sizeof(long)*(n+1));
    SA_IS(S, sa, n+1,256, sizeof(long)-1);
    long *lcp = (long*)malloc(sizeof(long)*(n+1));
    rank = (long*)malloc(sizeof(long)*(n+1));
    makelcp(S, sa, lcp, n+1);
    long ans = 0, c1=0,c2=0;
    for(long i=0;i<n+1;i++){
        if(lcp[i]>ans){
            ans = lcp[i];
            c1 = sa[i];
            c2 = sa[i+1];
        }
    }
    if(c1>c2){long tmp = c1;c1 = c2;c2=tmp;}
    printf("%ld %ld %ld\n",c1+1,c2+1, ans);
    return 0;
}
//高さ配列を生成
void makelcp(char* S, long *sa, long *lcp, long n){
    for(long i=0;i<n;i++) rank[sa[i]] = i;

    long h = 0;
    lcp[0] = 0;
    for(long i=0;i<n;i++){
        long j = sa[rank[i]-1];
        if(h>0) h--;
        for(;j+h<n && i+h<n;h++){
            if(S[j+h]!=S[i+h]) break;
        }
        lcp[rank[i]-1]=h;
    }
}