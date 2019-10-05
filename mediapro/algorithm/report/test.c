#include <stdio.h>
#include <stdlib.h>
int main(){
    char S[100];
    char c;
    int i=0;
    while((c = fgetc(stdin))!=EOF){
        S[i] = c;
        i++;
    }
    for(int j=0;j<i;j++) printf("%c ", S[j]);
    return 0;
}