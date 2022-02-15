#include "io.h"
#include "all.c"
char *line = NULL;
char *toy;
int choice = -1, id, idx = 0;
typedef struct hang_t{
    int id;
    char *toy;
}*hang;
void menu(){
    printf("1. Doc file A\n");
    printf("2. Doc file B\n");
    printf("3. Tim kiem\n");
    printf("4. Tong hop\n");
    printf("5. Thong ke\n");
    printf("6. Thoat\n");
}
int main(){
    rbm_t tree = rbm_create(gtype_cmp_l, NULL, NULL);
    rbm_t tree_toy = rbm_create(gtype_cmp_s, NULL, NULL);
    hang *hang_b = calloc(4, sizeof(hang));
    while(choice != 6){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            FILE *fa = fopen("a.txt", "r");
            while(cgetline(&line,0,fa)){
                id = atoi(strtok(line, "    \n"));
                toy = strtok(NULL, "    \n");
                if(toy == NULL){
                    printf("Nhap vao do choi cua %d: ", id);
                    cgetline(&toy,0,stdin);
                    remove_tail_lf(toy);
                }
                rbm_insert(tree, gtype_l(id), gtype_s(strdup(toy)));
            }
            rbm_traverse(k, v, tree){
                printf("%ld %s\n", k->l, v->s);
            }
            fclose(fa);
            printf("\n");
        }
        else if(choice == 2){
            FILE *fb = fopen("b.txt", "r");
            while(cgetline(&line,0,fb)){
                id = atoi(strtok(line, "    \n"));
                toy = strtok(NULL, "    \n");
                if(toy == NULL){
                    printf("Nhap vao do choi cua %d: ", id);
                    cgetline(&toy, 0, stdin);
                    remove_tail_lf(toy);
                }
                hang_b[idx] = malloc(sizeof(struct hang_t));
                hang_b[idx]->id = id;
                hang_b[idx]->toy = strdup(toy);
                idx++;
            }
            for(int i=0; i<idx; i++){
                printf("%d %s\n", hang_b[i]->id, hang_b[i]->toy);
            }
            fclose(fb);
            printf("\n");
        }
        else if(choice == 3){
            for(int i=0; i<idx; i++){
                if(rbm_remove(tree, gtype_l(hang_b[i]->id))){
                    printf("Em nho bi xoa: %d %s\n", hang_b[i]->id, hang_b[i]->toy);
                }
            }
            rbm_traverse(k, v, tree){
                printf("%ld %s\n",k->l, v->s);
            }
        }
        else if(choice == 4){
            for(int i=0; i<idx; i++){
                rbm_insert(tree, gtype_l(hang_b[i]->id), gtype_s(hang_b[i]->toy));
            }
            idx = 0;
            hang *hang_tmp = calloc(rbm_size(tree), sizeof(hang));
            rbm_traverse(k, v, tree){
                hang_tmp[idx] = malloc(sizeof(struct hang_t));
                hang_tmp[idx]->id = k->l;
                hang_tmp[idx]->toy = strdup(v->s);
                idx++;
            }
            for(int i = idx-1; i>=0; i--){
                printf("%d %s\n",hang_tmp[i]->id, hang_tmp[i]->toy);
            }
            free(hang_tmp);
        }
        else if(choice == 5){
            rbm_traverse(k, v, tree){
                rbm_ires res = rbm_insert(tree_toy, gtype_s(v->s), gtype_l(1));
                if(!res.inserted){
                    res.value->l++;
                }
            }
            rbm_traverse(k, v, tree_toy){
                printf("%s %ld\n", k->s, v->l);
            }
        }
    }
    free(line);
    free(toy);
    rbm_free(tree);
}