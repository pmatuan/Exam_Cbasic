#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "io.h"
#include "all.c"
int n = -1;
char *line = NULL;
int id = 0, idx = 0;
char *name;
gtype *tmp;
void menu(){
    printf("1. Doc file A.\n");
    printf("2. Doc file B.\n");
    printf("3. Tim kiem.\n");
    printf("4. Tong hop.\n");
    printf("5. Thong ke.\n");
    printf("6. Thoat\n");
}
int main(){
    bn_tree_t tree = bns_create_tree_g(NULL, gtype_cmp_l);
    hmap_t hmap = hmap_create(gtype_hash_l, gtype_cmp_l,NULL, NULL);
    hmap_t hmap_b = hmap_create(gtype_hash_l, gtype_cmp_l,NULL, NULL);
    int a[100]; /*đề không có số lượng, nên tạm thời như này nhé*/
    while(n!=6){
        menu();
        printf("Lua chon cua ban: ");
        scanf("%d",&n);
        clear_stdin();
        if(n == 1){
            FILE *f = fopen("a.txt", "r");
            while(cgetline(&line,0,f)){
                id = atoi(strtok(line, " \n"));
                name = strtok(NULL, " \n");
                if(name == NULL){
                    printf("Nhap qua cua %d: ",id);
                    cgetline(&name,0,stdin);
                    remove_tail_lf(name);
                }
                bns_insert_g(tree,gtype_l(id));
                hmap_insert(hmap, gtype_l(id), gtype_s(strdup(name)));
            }
            fclose(f);
            bn_traverse_lnr(cur, tree){
                id = bns_node_g_key(cur).l;
                tmp = hmap_value(hmap, gtype_l(id));
                printf("%d %s\n",id, tmp->s);
            }
        }
        else if (n == 2){
            FILE *f2 = fopen("b.txt", "r");
            while(cgetline(&line,0,f2)){
                id = atoi(strtok(line, " \n"));
                name = strtok(NULL, " \n");
                if(name == NULL){
                    printf("Nhap qua cua %d: ",id);
                    cgetline(&name,0,stdin);
                    remove_tail_lf(name);
                }
                a[idx] = id;
                hmap_insert(hmap_b, gtype_l(id), gtype_s(strdup(name)));
                idx++;
            }
            for(int i=0; i<idx; i++){
                tmp = hmap_value(hmap_b, gtype_l(a[i]));
                printf("%d %s\n",a[i],tmp->s);
            }
            fclose(f2);
        }
        else if(n == 3){
            hmap_traverse(key, value, hmap_b){
                if(hmap_value(hmap,gtype_l(key->l)) != NULL){
                    id = key->l;
                }
            }
            tmp = hmap_value(hmap, gtype_l(id));
            printf("Em nho bi trung: %d %s\n\n",id, tmp->s);
            bn_traverse_lnr(cur,tree){
                if(bns_node_g_key(cur).l == id){
                    bns_delete_g(tree,cur);
                }
            }
            bn_traverse_lnr(cur,tree){
                id = bns_node_g_key(cur).l;
                tmp = hmap_value(hmap, gtype_l(id));
                printf("%d %s\n",id, tmp->s);
            }
        }
        else if(n == 4){
            for(int i=0; i<idx; i++){
                tmp = hmap_value(hmap_b, gtype_l(a[i]));
                hmap_insert(hmap, gtype_l(a[i]), gtype_s(tmp->s));
                bns_insert_g(tree,gtype_l(a[i]));
            }
            bn_traverse_rnl(cur,tree){
                id = bns_node_g_key(cur).l;
                tmp = hmap_value(hmap, gtype_l(id));
                printf("%d %s\n", id, tmp->s);
            }
        }
        else if(n == 5){
            hmap_t out = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
            hmap_traverse(key, value, hmap){
                hmap_ires ires = hmap_insert(out, gtype_s(value->s), gtype_l(1));
                if(!ires.inserted){
                    ++(ires.value->l);
                }
            }
            hmap_traverse(key, value, out){
                printf("%s: %ld\n", key->s, value->l);
            }
        }
    }
}