#include "all.c"
#include "ext/io.h"
typedef struct bill_t{
    int id;
    char *name;
}*bill;
bill *list;
char *line = NULL, *name;
int choice = -1, id, idx = 0;
void menu(){
    printf("1. Doc file A\n");
    printf("2. Doc file B\n");
    printf("3. Tim kiem\n");
    printf("4. Tong hop\n");
    printf("5. Thong ke\n");
    printf("6. Thoat\n");
}
int main(){
    rbm_t tree = rbm_create(gtype_cmp_l, NULL, gtype_free_s);
    rbm_t tree_toy = rbm_create(gtype_cmp_s, NULL, NULL);
    while(choice != 6){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            FILE *fa = fopen("A.txt", "r");
            while(cgetline(&line,0,fa)){
                id = atoi(strtok(line, "\t\n "));
                name = strtok(NULL, "\t\n ");
                if(name == NULL){
                    printf("Nhap thong tin cua %d: ", id);
                    cgetline(&name,0,stdin);
                    remove_tail_lf(name);
                }
                rbm_insert(tree, gtype_l(id), gtype_s(strdup(name)));
            }
            rbm_traverse(k, v, tree){
                printf("%ld %s\n", k->l, v->s);
            }
            fclose(fa);
        }
        else if(choice == 2){
            FILE *fb = fopen("B.txt", "r");
            list = calloc(4, sizeof(bill));
            while(cgetline(&line,0,fb)){
                list[idx] = malloc(sizeof(struct bill_t));
                id = atoi(strtok(line, "\t\n "));
                name = strtok(NULL, "\t\n ");
                if(name == NULL){
                    printf("Nhap thong tin cua %d: ", id);
                    cgetline(&name,0,stdin);
                    remove_tail_lf(name);
                }
                list[idx]->id = id;
                list[idx]->name = strdup(name);
                idx++;
            }
            for(int i=0; i<4; i++){
                printf("%d %s\n", list[i]->id, list[i]->name);
            }
            fclose(fb);
        }
        else if(choice == 3){
            for(int i=0; i<4; i++){
                if(rbm_remove(tree, gtype_l(list[i]->id))){
                    printf("Em nho bi xoa: %d %s\n", list[i]->id, list[i]->name);
                }
            }
            rbm_traverse(k, v, tree){
                printf("%ld %s\n", k->l, v->s);
            }
        }
        else if(choice == 4){
            for(int i=0; i<4; i++){
                rbm_insert(tree, gtype_l(list[i]->id), gtype_s(list[i]->name));
            }
            bn_tree_t t = (bn_tree_t) tree;
            rbm_node_t tmp;
            bn_traverse_rnl(cur, t){
                tmp = (rbm_node_t)cur;
				printf("%ld %s\n", (tmp->key).l, (tmp->value).s);
            }
        }
        else if(choice == 5){
            rbm_traverse(k, v, tree){
                rbm_ires res = rbm_insert(tree_toy, gtype_s(v->s), gtype_l(1));
                if(!res.inserted) res.value->l++;
            }
            rbm_traverse(k, v, tree_toy){
                printf("%s: %ld\n", k->s, v->l);
            }
        }
    }
    rbm_free(tree);
    rbm_free(tree_toy);
    for(int i=0; i<4; i++){
        free(list[i]);
    }
    free(list);
    free(name);
    free(line);
    return 0;
}