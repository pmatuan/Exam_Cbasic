#include "all.c"
#include "io.h"
typedef struct SV_t{
    char *pass;
    double diem;
}*SV;
SV sinhvien;
gtype *tmp;
int choice = -1, count = 0, choice_user = -1, choice_admin = -1;
char *line = NULL, *user, *pass, *pass_2 = NULL;
double diem;
void menu(){
    printf("1. Dang nhap\n");
    printf("2. Thoat\n");
}
void menu_sv(){
    printf("1. Xem diem sinh vien do\n");
    printf("2. Thay doi mat khau\n");
    printf("3. Luu thong tin\n");
}
void menu_admin(){
    printf("1. Them sinh vien\n");
    printf("2. In danh sach\n");
    printf("3. Xoa sinh vien\n");
    printf("4. Luu thong tin\n");
}
int check_user(rbm_t tree, char *user){
    rbm_traverse(k, v, tree){
        if(!strcmp(k->s, user)) return 1;
    }
    return 0;
}
int check_pass(rbm_t tree, char *pass){
    rbm_traverse(k, v, tree){
        sinhvien = v->v;
        if(!strcmp(sinhvien->pass, pass)) return 1;
    }
    return 0;
}
void SINHVIEN(rbm_t tree, char *file){
    while(choice_user != 3){
        menu_sv();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice_user);
        clear_stdin();
        if(choice_user == 1){
            tmp = rbm_value(tree, gtype_s(user));
            sinhvien = tmp->v;
            printf("Diem sinh vien: %lf\n", sinhvien->diem);
        }
        else if(choice_user == 2){
            printf("Nhap mat khau can thay: ");
            cgetline(&pass,0,stdin);
            remove_tail_lf(pass);
            printf("Nhap lai mat khau: ");
            cgetline(&pass_2,0,stdin);
            remove_tail_lf(pass_2);
            while(strcmp(pass, pass_2)){
                printf("Khong khop! Nhap lai!\n");
                printf("Nhap mat khau can thay: ");
                cgetline(&pass,0,stdin);
                remove_tail_lf(pass);
                printf("Nhap lai mat khau: ");
                cgetline(&pass_2,0,stdin);
                remove_tail_lf(pass_2);
            }
            tmp = rbm_value(tree, gtype_s(user));
            sinhvien = tmp->v;
            sinhvien->pass = strdup(pass);
        }
        else if(choice_user == 3){
            FILE *f_user = fopen(file, "w+");
            rbm_traverse(k, v, tree){
                sinhvien = v->v;
                fprintf(f_user, "%s  %s  %lf\n", k->s, sinhvien->pass, sinhvien->diem);
            }
            fclose(f_user);
        }
    }
}
void ADMIN(rbm_t tree, char *file){
    while(choice_admin != 4){
        menu_admin();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d", &choice_admin);
        clear_stdin();
        if(choice_admin == 1){
            printf("Nhap ten user: ");
            cgetline(&user, 0, stdin);
            remove_tail_lf(user);
            printf("Nhap pass user: ");
            cgetline(&pass, 0, stdin);
            remove_tail_lf(pass);
            printf("Nhap diem: ");
            scanf("%lf", &diem);
            clear_stdin();
            sinhvien = malloc(sizeof(SV));
            sinhvien->pass = strdup(pass);
            sinhvien->diem = diem;
            rbm_insert(tree, gtype_s(strdup(user)), gtype_v(sinhvien));
        }
        else if(choice_admin == 2){
            rbm_traverse(k, v, tree){
                if(strcmp("Admin", k->s)){
                    sinhvien = v->v;
                    printf("%s %s %lf\n", k->s, sinhvien->pass, sinhvien->diem);
                }
            }
        }
        else if(choice_admin == 3){
            printf("Nhap user can xoa: ");
            cgetline(&user, 0, stdin);
            remove_tail_lf(user);
            while(!strcmp("Admin", user)){
                printf("Khong the xoa admin! Nhap lai!\n");
                printf("Nhap user can xoa: ");
                cgetline(&user, 0, stdin);
                remove_tail_lf(user);
            }
            tmp = rbm_value(tree, gtype_s(user));
            sinhvien = tmp->v;
            printf("%s %s %lf\n", user, sinhvien->pass, sinhvien->diem);
            rbm_remove(tree, gtype_s(user));
        }
        else if(choice_admin == 4){
            FILE *f_admin = fopen(file, "w+");
            rbm_traverse(k, v, tree){
                sinhvien = v->v;
                fprintf(f_admin, "%s  %s  %lf\n", k->s, sinhvien->pass, sinhvien->diem);
            }
            fclose(f_admin);
        }
    }
}
int main(){
    FILE *f = fopen("sinhvien.txt", "r");
    rbm_t tree = rbm_create(gtype_cmp_s, NULL, NULL);
    while(cgetline(&line,0,f)){
        sinhvien = malloc(sizeof(SV));
        user = strtok(line, "   \n");
        pass = strtok(NULL, " \n");
        diem = atof(strtok(NULL, " \n"));
        sinhvien->pass = strdup(pass);
        sinhvien->diem = diem;
        rbm_insert(tree, gtype_s(strdup(user)), gtype_v(sinhvien));
    }
    fclose(f);
    user = NULL, pass = NULL;
    while(choice != 2){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d", &choice);
        clear_stdin();
        if(choice == 2) break;
        while(count < 3){
            printf("Nhap ten user: ");
            cgetline(&user,0,stdin);
            remove_tail_lf(user);
            if(check_user(tree, user)){
                printf("Nhap password: ");
                cgetline(&pass, 0, stdin);
                remove_tail_lf(pass);
                if(!check_pass(tree, pass)){
                    count++;
                    if(count < 3) printf("Nhap sai! Nhap lai\n");
                    else printf("Nhap sai qua 3 lan!\n");
                    continue;
                }
                if(strcmp(user,"Admin")){
                    SINHVIEN(tree, "sinhvien.txt");
                    break;
                }
                else{
                    ADMIN(tree, "sinhvien.txt");
                    break;
                }
            }
            else{
                count++;
                if(count < 3) printf("Nhap sai! Nhap lai\n");
                else printf("Nhap sai qua 3 lan!\n");
            }
        }
        if(count == 3) break;
    }
    return 0;
}