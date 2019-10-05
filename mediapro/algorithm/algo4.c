#include <stdio.h>

int main(int argc, char ** argv){
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[1], "r");
    char c1, c2;
    long cnt, i=0, j, best = 0, cnt1 = 0, cnt2;
    //cnt1, 読んだ文字数
    while( (c1 = getc(f1)) != -1 ){
        cnt1 ++;
        fseek(f2, cnt1, SEEK_SET);
        while( (c2 = getc(f2)) != -1){
            cnt = 0;
            while( c1 == c2 ){
                c1 = getc(f1);
                c2 = getc(f2);
                cnt ++;
            }
            if(cnt > best){
                best = cnt;
            }
            fseek(f1, -1*cnt, SEEK_CUR);
            fseek(f2, -1*cnt, SEEK_CUR);
            if(c2 == -1){
                break;
            }
        }
        if(c1 == -1){
            break;
        }
    }
    printf("%ld\n", best);
    return 0;
}