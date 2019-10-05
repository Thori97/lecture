#include <iostream> 
#include <cstring> 
#include <algorithm> 
using namespace std; 

// Structure to store information of a suffix 
struct suffix 
{ 
    int index; // To store original index 
    int rank[2]; // To store ranks and next rank pair 
}; 
long cmp(struct suffix a, struct suffix b) 
{ 
    return (a.rank[0] == b.rank[0])? (a.rank[1] < b.rank[1] ?1: 0): 
               (a.rank[0] < b.rank[0] ?1: 0); 
} 
long *buildSuffixArray(char *txt, long n) 
{ 
    // A structure to store suffixes and their indexes 
    struct suffix suffixes[n];
  
    // Store suffixes and their indexes in an array of structures. 
    // The structure is needed to sort the suffixes alphabatically 
    // and maintain their old indexes while sorting 
    for (long i = 0; i < n; i++) 
    { 
        suffixes[i].index = i; 
        suffixes[i].rank[0] = txt[i] - 'a'; 
        suffixes[i].rank[1] = ((i+1) < n)? (txt[i + 1] - 'a'): -1; 
    } 
  
    // Sort the suffixes using the comparison function 
    // defined above. 
    sort(suffixes, suffixes+n, cmp); 
  
    // At this point, all suffixes are sorted according to first 
    // 2 characters.  Let us sort suffixes according to first 4 
    // characters, then first 8 and so on 
    long ind[n];  // This array is needed to get the index in suffixes[] 
                 // from original index.  This mapping is needed to get 
                 // next suffix. 
    for (long k = 4; k < 2*n; k = k*2) 
    { 
        // Assigning rank and index values to first suffix 
        long rank = 0; 
        long prev_rank = suffixes[0].rank[0]; 
        suffixes[0].rank[0] = rank; 
        ind[suffixes[0].index] = 0; 
  
        // Assigning rank to suffixes 
        for (long i = 1; i < n; i++) 
        { 
            // If first rank and next ranks are same as that of previous 
            // suffix in array, assign the same new rank to this suffix 
            if (suffixes[i].rank[0] == prev_rank && 
                    suffixes[i].rank[1] == suffixes[i-1].rank[1]) 
            { 
                prev_rank = suffixes[i].rank[0]; 
                suffixes[i].rank[0] = rank; 
            } 
            else // Otherwise increment rank and assign 
            { 
                prev_rank = suffixes[i].rank[0]; 
                suffixes[i].rank[0] = ++rank; 
            } 
            ind[suffixes[i].index] = i; 
        } 
  
        // Assign next rank to every suffix 
        for (long i = 0; i < n; i++) 
        { 
            long nextindex = suffixes[i].index + k/2; 
            suffixes[i].rank[1] = (nextindex < n)? 
                                  suffixes[ind[nextindex]].rank[0]: -1; 
        } 
  
        // Sort the suffixes according to first k characters 
        sort(suffixes, suffixes+n, cmp); 
    } 
  
    // Store indexes of all sorted suffixes in the suffix array 
    long *suffixArr = new long[n]; 
    for (long i = 0; i < n; i++) 
        suffixArr[i] = suffixes[i].index; 
  
    // Return the suffix array 
    return  suffixArr; 
}
long n;
long *sa;
long *tmp;

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
    
    tmp = (long*)malloc(sizeof(long)*(n+1));
    if(tmp == NULL){
        printf("tmpNG\n");
    }
    printf("\n");
    long *a = buildSuffixArray(S, n);
    for(long i=0;i<n+1;i++){
        printf("%ld ",sa[i]);
    }
    free(S);
    free(sa);
    free(tmp);
    return 0;
}