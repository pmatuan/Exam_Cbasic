#include "all.c"
#include "ext/io.h"
#include "limits.h"
typedef char string[4];
typedef struct CK_t{
    string id;
    double open;
    double close;
}*CK;
int n, choice = -1;
CK ck;
char *line = NULL;
void menu(){
    printf("1. Đọc file\n");
    printf("2. Tìm kiếm theo mã chứng khoán\n");
    printf("3. Tìm kiếm những mã chứng khoán có xu hướng tăng\n");
    printf("4. Tìm mã có số ngày tăng lớn nhất\n");
    printf("5. Thoát chương trình\n");
}

int main(){
    gvec_t vec = gvec_create(10, gtype_free_v);
    rbm_t tree = rbm_create(gtype_cmp_s, NULL, NULL);
    FILE *f = fopen("data.txt", "r");
    fscanf(f, "%d", &n);
    while(choice != 5){
        menu();
        printf("Nhập vào lựa chọn của bạn: ");
        scanf("%d", &choice);
        clear_stdin();
        if(choice == 1){
            for(int i=0; i<n*10; i++){
                ck = malloc(sizeof(struct CK_t));
                fscanf(f, "%s %lf %lf", ck->id, &ck->open, &ck->close);
                gvec_append(vec, gtype_v(ck));
            }
            gvec_traverse(cur, vec){
                ck = cur->v;
                rbm_ires res = rbm_insert(tree, gtype_s(ck->id), gtype_d(ck->close - ck->open));
                if(!res.inserted){
                    res.value->d += (ck->close - ck->open);
                }
            }
            rbm_traverse(k, v, tree){
                printf("%s %.3lf\n", k->s, v->d/10);
            }
        }
        else if(choice == 2){
            double max = (double) INT_MIN;
            double min = (double) INT_MAX;
            printf("Nhập vào mã cổ phiếu: ");
            cgetline(&line, 0, stdin);
            remove_tail_lf(line);
            gvec_traverse(cur, vec){
                ck = cur->v;
                if(!strcmp(ck->id, line)){
                    if(ck->close > max) max = ck->close;
                    if(ck->close < min) min = ck->close;
                }
            }
            if(max == INT_MIN){
                printf("Không tìm thấy\n");
            }
            else{
                printf("Giá đóng cửa cao nhất: %.3lf\n", max);
                printf("Giá đóng cửa thấp nhất: %.3lf\n", min);
            }
        }
        else if(choice == 3){
            int count = 0;
            hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
            gvec_traverse(cur, vec){
                if(count == 2*n) break;
                ck = cur->v;
                if(ck->close > ck->open){
                    hmap_ires res = hmap_insert(map, gtype_s(ck->id), gtype_l(1));
                    if(!res.inserted){
                        res.value->l++;
                    }
                }
                count++;
            }
            hmap_traverse(k, v, map){
                if(v->l == 2){
                    printf("%s ", k->s);
                }
            }
            printf("\n");
            hmap_free(map);
        }
        else if(choice == 4){
            int max = INT_MIN;
            hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
            gvec_traverse(cur, vec){
                ck = cur->v;
                 if(ck->close > ck->open){
                    hmap_ires res = hmap_insert(map, gtype_s(ck->id), gtype_l(1));
                    if(!res.inserted){
                        res.value->l++;
                    }
                }
            }
            hmap_traverse(k, v, map){
                if(max < v->l) max = v->l;
            }
            printf("Số ngày tăng lớn nhất: %d\n", max);
            hmap_traverse(k, v, map){
                if(max == v->l){
                    printf("%s ", k->s);
                }
            }
            printf("\n");
            hmap_free(map);
        }
        else if(choice == 5) printf("Phan Minh Anh Tuấn - 20205227 - Sửa lại lỗi lầm hôm thi CK :((\n");
    }
    fclose(f);
    rbm_free(tree);
    gvec_free(vec);
    free(line);
    return 0;
}