#include "all.c"
#include "ext/io.h"
typedef char string[21];
typedef struct bill_t{
    char *id;
    int price;
}*bill;
string id;
char *line = NULL;
int n, price, m, quantity;
int cmp(const void *a, const void *b){
    return (*(const bill*)b)->price - (*(const bill*)a)->price; 
}
int main(int argc, char *argv[]){
    hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, gtype_free_s, NULL);
    FILE *f1 = fopen(argv[1], "r");
    fscanf(f1, "%d", &n);
    while(!feof(f1)){
        fscanf(f1, "%s %d", id, &price);
        char *tmp = strdup(id);
        hmap_ires res = hmap_insert(map, gtype_s(tmp), gtype_l(price));
        if(!res.inserted) free(tmp);
    }
    fclose(f1);
    FILE *f2 = fopen(argv[2], "r");
    n = atoi(cgetline(&line,0,f2));
    bill *b = calloc(n, sizeof(bill));
    for(int i=0; i<n; i++){
        price = 0;
        b[i] = malloc(sizeof(struct bill_t));
        cgetline(&line,0,f2);
        b[i]->id = strdup(strtok(line, " \n"));
        m = atoi(strtok(NULL, " \n"));
        for(int j=0; j<m; j++){
            price += hmap_value(map, gtype_s(strtok(NULL, " \n")))->l * atoi(strtok(NULL, " \n"));
        }
        b[i]->price = price;
    }
    fclose(f2);
    qsort(b, n, sizeof(b), cmp);
    for(int i=0; i<3; i++){
        printf("%s %d\n",b[i]->id, b[i]->price);
    }
    for(int i=0; i<n; i++){
        free(b[i]->id);
        free(b[i]);
    }
    free(b);
    free(line);
    hmap_free(map);
    return 0;
}