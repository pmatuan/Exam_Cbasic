#include "all.c"
#include "ext/io.h"
#include "limits.h"
typedef char string[4];
char *line = NULL;
int choice = -1, n;
typedef struct CK_t{
    string id;
    double open;
    double close;
}*CK;
CK ck;
void menu(){
    printf("1. Doc file\n");
    printf("2. Tim kiem theo ma chung khoan\n");
    printf("3. Tim kiem nhung ma chung khoan co xu huong tang\n");
    printf("4. Tim ma so co so ngay tang lon nhat\n");
    printf("5. Thoat\n");
}
int main(int argc, char *argv[]){
    gvec_t vec = gvec_create(10, gtype_free_v);
    rbm_t tree = rbm_create(gtype_cmp_s, NULL, NULL);
    while(choice != 5){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            FILE *f = fopen(argv[1], "r");
            fscanf(f, "%d", &n);
            for(int i=0; i<10; i++){
                for(int j=0; j<n; j++){
                    ck = malloc(sizeof(struct CK_t));
                    fscanf(f, "%s   %lf %lf", ck->id, &ck->open, &ck->close);
                    gvec_append(vec, gtype_v(ck));
                }
            }
            gvec_traverse(cur, vec){
                ck = cur->v;
                rbm_ires res = rbm_insert(tree, gtype_s(ck->id), gtype_d(ck->close - ck->open));
                if(!res.inserted){
                    res.value->d += ck->close - ck->open;
                }
            }
            rbm_traverse(k, v, tree){
                printf("%s  %.3lf\n", k->s, v->d/10);
            }
            fclose(f);
        }
        else if(choice == 2){
            double max = (double)INT_MIN, min = (double)INT_MAX;
            printf("Nhap vao ma co phieu: ");
            cgetline(&line, 0, stdin);
            remove_tail_lf(line);
            if(rbm_value(tree, gtype_s(line)) == NULL){
                printf("Khong tim thay ma chung khoan\n");
            }
            else{
                gvec_traverse(cur, vec){
                    ck = cur->v;
                    if(!strcmp(line, ck->id)){
                        if(max < ck->close){
                            max = ck->close;
                        }
                        if(min > ck->close){
                            min = ck->close;
                        }
                    }
                }
                printf("%s co gia dong cua cao nhat la: %.3lf\n", line, max);
                printf("%s co gia dong cua thap nhat la: %.3lf\n", line, min);
            }
        }
        else if(choice == 3){
            int count = 0;
            hmap_t h = hmap_create(gtype_hash_s,gtype_cmp_s,NULL,NULL);
            gvec_traverse(cur,vec){
                ck = cur->v;
                if(ck->close > ck->open){
                    hmap_ires res = hmap_insert(h,gtype_s(ck->id),gtype_l(1));
                    if(!res.inserted){
                        if(ck->close > ck->open) res.value->l++;
                    }
                }
                count++;
                if(count==n*2) break;
            }
            hmap_traverse(key,val,h){
                if(val->l==2){
                    printf("%s\t",key->s);
                    break;
                }
            }
            printf("\n");
            hmap_free(h);
        }
        else if(choice == 4){
            int max = INT_MIN;
            gvec_t vec_tmp = gvec_create(10, NULL);
            hmap_t hmap = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
            gvec_traverse(cur, vec){
                ck = cur->v;
                if(ck->close > ck->open){
                    hmap_ires res = hmap_insert(hmap, gtype_s(ck->id), gtype_l(1));
                    if(!res.inserted){
                        res.value->l++;
                    }
                }
            }
            hmap_traverse(k, v, hmap){
                if(max < v->l) max = v->l;
            }
            printf("So ngay lon nhat: %d\n", max);
            hmap_traverse(k, v, hmap){
                if(max == v->l){
                    gvec_append(vec_tmp, gtype_s(k->s));
                }
            }
            gvec_traverse(cur, vec_tmp){
                printf("%s ", cur->s);
            }
            printf("\n");
            gvec_free(vec_tmp);
            hmap_free(hmap);
        }
        else if(choice == 5) printf("Tac gia: Phan Minh Anh Tuan - 20205227\n");
    }
    free(line);
    gvec_free(vec);
    rbm_free(tree);
    return 0;
}