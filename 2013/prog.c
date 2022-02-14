#include "all.c"
#include "io.h"
typedef char string[11];
int a, b, choice = -1, threshold = 1;
string team1, team2, team;
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
    FILE *f = fopen(argv[1], "r");
    rbm_t tree = rbm_create(gtype_cmp_s, NULL, NULL);
    while(choice!=6){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        if(choice == 1){
            while(!feof(f)){
                fscanf(f,"%s\t%s\t%d\t%d", team1, team2, &a, &b);
                rbm_ires res1 = rbm_insert(tree,gtype_s(strdup(team1)), gtype_l(0));
                rbm_ires res2 = rbm_insert(tree,gtype_s(strdup(team2)), gtype_l(0));
                if(a > b) res1.value->l+=3;
                else if(a < b) res2.value->l+=3;
                else {
                    res1.value->l+=1;
                    res2.value->l+=1;
                }
            }
        }
        else if(choice == 2){
            rbm_traverse(k, v, tree){
                printf("%s %ld\n",k->s,v->l);
            }
        }
        else if(choice == 3){
            printf("Nhap ma doi bong: ");
            scanf("%s",team);
            clear_stdin();
            tmp = rbm_value(tree, gtype_s(team));
            if(tmp == NULL){
                printf("Khong co doi bong nay\n");
            }
            else printf("%s %ld\n",team, tmp->l);
        }
        else if(choice == 4){
            gvec_t vec = gvec_create(10, NULL);
            printf("Nhap vao nguong: ");
            scanf("%d",&threshold);
            rbm_traverse(k, v, tree){
                if(v->l < threshold){
                    gvec_append(vec, gtype_s(k->s));
                }
            }
            gvec_traverse(cur, vec){
                printf("Doi bong bi xuong hang la %s\n", cur->s);
                rbm_remove(tree, gtype_s(cur->s));
            }
            rbm_traverse(k, v, tree){
                printf("%s %ld\n", k->s, v->l);
            }
        }
        else if(choice == 5){
            FILE *out = fopen(argv[2], "w+");
            rbm_traverse(k, v, tree){
                fprintf(out, "%s\t%ld\n", k->s, v->l);
            }
            fclose(out);
        }
    }
    rbm_free(tree);
    fclose(f);
}