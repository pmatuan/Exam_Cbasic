#include "all.c"
#include "io.h"
typedef char string[11];
int a,b;
string a1, b1;
char *name, *name_1;
int choice = -1, count = 0, X;
gtype *tmp;
void menu(){
    printf("1. Tao cay\n");
    printf("2. Ket qua\n");
    printf("3. Tim kiem\n");
    printf("4. Xuong hang\n");
    printf("5. Xuat file\n");
    printf("6. Thoat\n");
}
int main(int argc, char *argv[]){
    hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
    bn_tree_t tree = bns_create_tree_g(NULL, gtype_cmp_s);
    FILE *f = fopen(argv[1], "r");
    while(!feof(f)){
        fscanf(f, "%s   %s  %d  %d", a1, b1, &a, &b);
        hmap_ires res1 = hmap_insert(map, gtype_s(strdup(a1)), gtype_l(0));
        hmap_ires res2 = hmap_insert(map, gtype_s(strdup(b1)), gtype_l(0));
        if(a > b){
            res1.value->l += 3;
        }
        else if(a < b){
            res2.value->l += 3;
        }
        else if(a == b){
            res1.value->l += 1;
            res2.value->l += 1;
        }
    }
    while(choice != 6){
        menu();
        printf("Lua chon cua ban: ");
        scanf("%d",&choice);
        if(choice == 1){
            hmap_traverse(key,value,map){
                count += 1;
                bns_insert_g(tree, gtype_s(key->s));
                printf("Nut: %s, nut thu %d, dia chi cua nut: %p\n", key->s, count, key->s);
            }
        }
        else if(choice == 2){
            bn_traverse_lnr(cur,tree){
                name = bns_node_g_key(cur).s;
                tmp = hmap_value(map, gtype_s(strdup(name)));
                printf("%s %ld\n",name, tmp->l);
            }
        }
        else if(choice == 3){
            clear_stdin();
            printf("Nhap vao ma doi can tim: ");
            cgetline(&name_1, 0, stdin);
            remove_tail_lf(name_1);
            tmp = hmap_value(map, gtype_s(strdup(name_1)));
            if(tmp == NULL){
                printf("Khong co doi bong nay!\n");
            }
            else{
                printf("Diem doi nay la: %ld\n",tmp->l);
            }
        }
        else if(choice == 4){
            printf("Nhap X: ");
            scanf("%d", &X);
            bn_traverse_lnr(cur, tree){
                name = bns_node_g_key(cur).s;
                tmp = hmap_value(map, gtype_s(strdup(name)));
                if(X > tmp->l){
                    printf("%s\n", bns_node_g_key(cur).s);
                    bns_delete_g(tree, cur);
                }
            }
            printf("%ld\n",bn_size(tree));
        }
        else if(choice == 5){
            FILE *f2 = fopen(argv[2], "w+");
            bn_traverse_lnr(cur, tree){
                name = bns_node_g_key(cur).s;
                tmp = hmap_value(map, gtype_s(name));
                fprintf(f2, "%s\t%ld\n",name, tmp->l);
            }
            fclose(f2);
        }
    }
    fclose(f);
}
