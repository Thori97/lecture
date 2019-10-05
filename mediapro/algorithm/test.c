#include <stdio.h>

int main(int argc, char ** argv){
    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 4, SEEK_SET);
    char c = getc(fp);
    printf("%c\n", c);
}