#include "all.c"
#include "io.h"
typedef struct Point_t{
    char *diadiem;
    int gio;
    int phut;
}*Point;
Point point, point_1;
int choice = -1, gio = -1, phut = -1;
char *line, *diadiem;
void menu(){
    printf("CHUONG TRINH TRUY VET COVID19\n");
    printf("1. Nap file log lich su di chuyen\n");
    printf("2. In ra lich su di chuyen\n");
    printf("3. Tim kiem lich su di chuyen theo dia diem\n");
    printf("4. Tim kiem lich su di chuyen theo thoi gian\n");
    printf("5. Kiem tra truy vet moi nhat\n");
    printf("6. Thoat\n");
}
int main(){
    gsl_t list = gsl_create(gtype_free_v);
    while(choice != 6){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d", &choice);
        clear_stdin();
        if(choice == 1){
            FILE *f = fopen("inp.txt", "r");
            while(cgetline(&line,0,f)){
                point = malloc(sizeof(struct Point_t));
                point->diadiem = strdup(strtok(line, " \n"));
                point->gio = atoi(strtok(NULL, "   \n"));
                point->phut = atof(strtok(NULL, " \n"));
                gsl_push_back(list, gtype_v(point));
            }
            fclose(f);
        }
        else if(choice == 2){
            printf("%20s %10s %10s\n", "Dia diem", "Gio", "Phut");
            gsl_traverse(cur, list){
                point = cur->v;
                printf("%20s %10d %10d\n", point->diadiem, point->gio, point->phut);
            }
        }
        else if(choice == 3){
            int count = 0;
            printf("Nhap vao dia diem: ");
            cgetline(&line, 0, stdin);
            remove_tail_lf(line);
            gsl_traverse(cur, list){
                point = cur->v;
                if(!strcmp(line, point->diadiem)){
                    if(count != 0) printf(", ");
                    printf("%d:%d",point->gio, point->phut);
                    count++;
                }
            }
            if(count == 0) printf("Ban chua toi dia diem do\n");
            else printf("\n");
        }
        else if(choice == 4){
            int count = 0;
            while(gio < 0 || gio >= 24 || phut < 0 || phut >= 60){
                if(count > 0) printf("Nhap lai!\n");
                printf("Nhap vao gio: ");
                scanf("%d", &gio);
                printf("Nhap vao phut: ");
                scanf("%d",&phut);
                count++;
            }
            count = 0;
            gsl_traverse(cur, list){
                point = cur->v;
                if(gio == point->gio && phut == point->phut){
                    printf("%s\n", point->diadiem);
                    count++;
                    break;
                }
            }
            if(count == 0) printf("KHONG tim thay lich su di chuyen!\n");
        }
        else if(choice == 5){
            int count = 0;
            printf("Nhap thong tin cua benh nhan: ");
            cgetline(&line, 0, stdin);
            remove_tail_lf(line);
            diadiem = strtok(line, " \n");
            gio = atoi(strtok(NULL, " \n"));
            phut = atoi(strtok(NULL, " \n"));
            gsl_traverse(cur, list){
                if(strcmp(point->diadiem, diadiem)) point = cur->v;
                else{
                    point_1 = cur->v;
                    if(gio < point_1->gio && gio > point->gio){
                        printf("Ban co kha nang bi lay Covid, can phai khai bao y te ngay lap tuc!\n");
                    }
                    else if(gio < point->gio){
                        printf("Ban co kha nang bi lay Covid, can phai khai bao y te ngay lap tuc!\n");
                    }
                    else if(gio > point_1->gio){
                        printf("Lich su di chuyen cua ban OK\n");
                    }
                    else if(gio == point_1->gio){
                        if(phut < point_1->phut) printf("Ban co kha nang bi lay Covid, can phai khai bao y te ngay lap tuc!\n");
                        else printf("Lich su di chuyen cua ban OK\n");
                    }
                    else if(gio == point->gio){
                        printf("Ban co kha nang bi lay Covid, can phai khai bao y te ngay lap tuc!\n");
                    }
                }
            }
        }
    }
    gsl_free(list);
    free(line);
    free(diadiem);
    free(point);
    free(point_1);
    return 0;
}