#include <stdio.h>
#include <stdlib.h>
int main(int argc, char ** argv){
    FILE *f1 = fopen(argv[1], "r"), *f2 = fopen(argv[1], "r");
    long k = 0, ans = 0, cnt = 0, cnt1 =0, cnt2=0;//文字数カウント
    char c1, c2;
    while(1){//kのループ
        k ++;
        /*char buf*;
        buf = (char *)malloc(sizeof(char)* k);*/

        cnt1=0 , cnt2=0;
        while( (c1 = getc(f1)) != EOF){//c1のループ
        fseek(f2, cnt1+1, SEEK_SET);
        while( (c2 = getc(f2)) != EOF){//c2のループ
            for(cnt = 0; cnt<k; cnt++){
                //printf("%c %c\n", c1, c2);
                if(c2 == c1){
                    c1 = getc(f1);
                    c2 = getc(f2);
                    if(cnt+1 == k){
                        ans = k;
                    }
                }
                else{
                    break;
                }
            }
            cnt ++;
            fseek(f2, cnt1+cnt2-cnt, SEEK_SET);
        }
        cnt1 ++;
        if(ans == k){
            break;
        }
        }
    if(ans<k){
        break;
    }
    }
    printf("%ld\n", ans);
}