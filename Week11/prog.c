#include "all.c"
#include "ext/io.h"
typedef struct hang_t{
    char *name;
    double price;
    int quantity;
}*hang;
typedef struct bill_t{
    char *name;
    double price;
    int quantity;
    double total;
}*bill;
void menu(){
    printf("1. Xem thong tin mat hang\n");
    printf("2. Tao don hang\n"); 
    printf("3. Thoat\n");
}
int choice = -1, n, quantity, idx = 1;
double tong = 0;
bill bi;
char *line = NULL;
hang *h;
gtype *g;
int main(int argc, char *argv[]){
    hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, gtype_free_s, NULL);
    gsl_t list = gsl_create(gtype_free_v);
    FILE *f = fopen(argv[1], "r");
    n = atoi(cgetline(&line,0,f));
    h = calloc(n,sizeof(hang));
    for(int i=0; i<n; i++){
        h[i] = malloc(sizeof(struct hang_t));
        cgetline(&line, 0, f);
        char *tmp = strdup(strtok(line, " \n"));
        h[i]->name = strdup(strtok(NULL, " \n"));
        h[i]->price = atof(strtok(NULL, " \n"));
        h[i]->quantity = atoi(strtok(NULL, " \n"));
        hmap_ires res = hmap_insert(map, gtype_s(tmp), gtype_v(h[i]));
        if(!res.inserted) free(tmp);
    }
    fclose(f);
    while(choice != 3){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            printf("Nhap ma so mat hang: ");
            cgetline(&line, 0, stdin);
            remove_tail_lf(line);
            g = hmap_value(map, gtype_s(line));
            if(g){
                hang tmp = g->v;
                printf("%10s %20s %20s %20s\n", "Ma so", "Ten mat hang", "Gia tien", "So luong");
                printf("%10s %20s %20lf %20d\n", line, tmp->name, tmp->price, tmp->quantity);
            }
            else printf("Khong tim thay\n");
        }
        else if(choice == 2){
            while(strcmp(line, "STOP")){
                printf("Nhap ma so mat hang: ");
                cgetline(&line, 0, stdin);
                remove_tail_lf(line);
                g = hmap_value(map, gtype_s(line));
                if(g){
                    hang tmp = g->v;
                    printf("Nhap vao so luong hang: ");
                    scanf("%d", &quantity);
                    clear_stdin();
                    if(quantity > tmp->quantity) printf("Khong du hang\n");
                    else{
                        bi = malloc(sizeof(struct bill_t));
                        bi->name = tmp->name;
                        bi->price = tmp->price;
                        bi->quantity = quantity;
                        bi->total = tmp->price * quantity;
                        gsl_push_back(list, gtype_v(bi));
                    }
                }
                else if(!strcmp(line, "STOP")) break;
                else printf("Khong co mat hang!\n");
            }
            printf("Cty ABC\n");
            printf("%30s", "Don hang ban le\n");
            printf("%5s %20s %20s %20s %20s\n", "STT", "Mat hang", "Don gia", "So luong", "Thanh tien");
            gsl_traverse(cur, list){
                bi = cur->v;
                printf("%5d %20s %20lf %20d %20lf\n", idx, bi->name, bi->price, bi->quantity, bi->total);
                tong += bi->total;
                idx++;
            }
            printf("Tong tien: %lf\n", tong);
        }
    }
    for(int i=0; i<n; i++){
        free(h[i]->name);
        free(h[i]);
    }
    free(h);
    free(line);
    gsl_free(list);
    hmap_free(map);
    return 0;
}