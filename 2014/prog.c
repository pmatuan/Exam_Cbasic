#include "all.c"
#include "io.h"
char *user = NULL, *pass = NULL;
int c1 = -1, count = 0;
void menu1(){
    printf("1. Dang nhap\n");
    printf("2. Thoat\n");
}
int main(){
    FILE *f = fopen("sinhvien.txt", "r");
    fclose(f);
    while(c1 != 2){
        menu1();
        cgetline(&user, 0, stdin);
        cgetline(&pass, 0, stdin);
    }
}