#include "all.c"
#include "io.h"
typedef struct team_t{
    int id;
    char *name;
    int point;
    int win;
    int lose;
}*team;
typedef struct pair_t{
    int x;
    int y;
}*pair;
typedef struct diem_t{
    int diem;
    int thang;
    int thua;
}*Diem;
team t;
pair vong;
Diem diem;
char *line;
int n, choice = -1, x, y;
gtype *tmp;
void menu(){
    printf("1. In thong tin cac doi bong\n");
    printf("2. In thong tin lich thi dau\n");
    printf("3. Cap nhat ket qua vong dau\n");
    printf("4. Thong ke\n");
    printf("5. Thoat\n");
}
void bang(){
    printf("%2s %20s %10s %10s %10s\n","ID", "Ten doi bong", "Diem", "So ban thang", "So ban thua");
}
int main(){
    gsl_t list = gsl_create(gtype_free_v);
    hmap_t map = hmap_create(gtype_hash_l, gtype_cmp_l, NULL, gtype_free_s);
    FILE *f = fopen("bongda.txt", "r");
    n = atoi(cgetline(&line,0,f));
    for(int i=0; i<n; i++){
        cgetline(&line,0,f);
        t = malloc(sizeof(struct team_t));
        t->id = atoi(strtok(line, " \n"));
        t->name = strdup(strtok(NULL, "\n"));
        t->point = 0;
        t->win = 0;
        t->lose = 0;
        gsl_push_back(list, gtype_v(t));
        hmap_insert(map, gtype_l(t->id), gtype_s(t->name));
    }
    gvec_t vec = gvec_create(n, gtype_free_v);
    for(int i=0; i<((n-1)*(n/2+1)); i++){
        cgetline(&line,0,f);
        if(!strncmp("Vong", line, 4)) continue;
        vong = malloc(sizeof(pair));
        vong->x = atoi(strtok(line, "   \n"));
        vong->y = atoi(strtok(NULL, " \n"));
        gvec_append(vec, gtype_v(vong));
    }
    while(choice != 5){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            gsl_traverse(cur, list){
                t = cur->v;
                bang();
                printf("%2d %20s %10d %10d %10d\n", t->id, t->name, t->point, t->win, t->lose);
            }
        }
        else if(choice == 2){
            int idx = 1, count = 0;
            gvec_traverse(cur, vec){
                if(count % (n/2) == 0){
                    printf("Vong %d\n", idx);
                    idx++;
                }
                count++;
                vong = cur->v;
                tmp = hmap_value(map, gtype_l(vong->x));
                printf("%s ", tmp->s);
                tmp = hmap_value(map, gtype_l(vong->y));
                printf("- %s\n", tmp->s);
            }
        }
        else if(choice == 3){
            int idx = 1, count = 0;
            hmap_t hmap = hmap_create(gtype_hash_l, gtype_cmp_l, NULL, gtype_free_v);
            gvec_traverse(cur, vec){
                if(count % (n/2) == 0){
                    printf("Cap nhat ket qua vong %d\n", idx);
                    idx++;
                }
                count++;
                vong = cur->v;
                tmp = hmap_value(map, gtype_l(vong->x));
                printf("%s ", tmp->s);
                tmp = hmap_value(map, gtype_l(vong->y));
                printf("- %s: ", tmp->s);
                scanf("%d - %d", &x, &y);
                if(x > y){
                    ;
                }
                else if(x < y);
                else{
;
                }
            }
            hmap_free(hmap);
        }
    }
    fclose(f);
    hmap_free(map);
    gsl_free(list);
    return 0;
}