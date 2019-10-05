#include <stdio.h>
#include <stdlib.h>
#define N 1000

int main(int argc, char ** argv){
    FILE *fp1 = fopen(argv[1], "r");
    FILE *fp2 = fopen(argv[1], "r");
    char chr1, chr2;
    long i=0, j = 0;
    int ans = 0, cnt = 0, l = 0;
    while( (chr1 = fgetc(fp1)) != 0 ){
        cnt = 0;
        fseek(fp2, i, 0);
        if( (chr2 = fgetc(fp2)) == chr1){
            cnt += 1;
            while(1){
                if( (chr1 = fgetc(fp1)) == (chr2 = fgetc(fp2))){
                    cnt += 1;
                }
                else{
                    break;
                }
            }
        }
        fseek(fp1, -cnt, i);
        fseek(fp2, -cnt, i);
        i++;
        printf("%ld\n", i);
    }
    return 0;
}