/*
    Bài tập tuần 11 Cbasic
*/
#include "all.c"
#include "io.h"
/*Cấu trúc mặt hàng*/
typedef struct hang_t{
    char *id;
    char *name;
    double price;
    int quantity;
}*hang;
/*Cấu trúc đơn hàng bài 2*/
typedef struct bill_t{
    char *name;
    double price;
    int quantity;
    double total;
}*bill;
/*Là 1 hàm đầu vào của GSL*/
void gsl_free_bill(gtype v){
    bill pp = v.v;
    free(pp->name);
    free(pp);
}
/*
    n là số lượng mặt hàng
    idx là chỉ số
    choice là lựa chọn của người dùng
    quantity là số lượng (một biến sẽ dùng trong bài 2)
*/
int n, idx = 0, choice = -1, quantity;
char *line = NULL;
void menu(){
    printf("1. Xem thong tin mat hang\n");
    printf("2. Tao don hang\n");
    printf("3. Thoat\n");
}
/*
    Bài 1: tìm kiếm => có thể sử dụng hmap, ưu điểm trong bài này là nhanh, nhược điểm là tốn bộ nhớ (khoảng hơn 3gb cho bài này)
        Cấu trúc hmap trong bài: map["id"] = struct có id giống với id trong []
    Bài 2: cho các mặt hàng vào đơn hàng, sử dụng linked list có ưu điểm đỡ phải realloc so với mảng, theo tôi linked list là 
    tối ưu cho bài này
*/
int main(){
    FILE *f = fopen("inp20k.txt", "r");
    cgetline(&line,0,f);
    n = atoi(line);
    hang *h = calloc(n,sizeof(hang));
    hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
    while(cgetline(&line,0,f)){
        h[idx] = malloc(sizeof(struct hang_t));
        h[idx]->id = strdup(strtok(line, " "));
        h[idx]->name = strdup(strtok(NULL, " "));
        h[idx]->price = atof(strtok(NULL, " "));
        h[idx]->quantity = atoi(strtok(NULL, " "));
        hmap_insert(map,gtype_s(h[idx]->id), gtype_v(h[idx])); /*Thêm khóa và giá trị vào map*/
        idx++;
    }
    while(choice != 3){
        menu();
        printf("Nhap vao lua chon cua ban: ");
        scanf("%d",&choice);
        clear_stdin();
        if(choice == 1){
            printf("Nhap ID: ");
            cgetline(&line,0,stdin);
            remove_tail_lf(line);
            gtype *value = hmap_value(map, gtype_s(line)); /*Lấy giá trị từ khóa line trong map*/
            if(value){
                /*
                    Sở dĩ mình có thể làm với kiểu dữ liệu bất kì là do con trỏ (void *) có thể ép cho mọi kiểu
                    Để truy cập vào các thành phần trong struct mình định nghĩa, nhớ ép kiểu về đúng dạng nhé
                */
                hang p1 = value->v; 
                printf("%s %s %lf %d\n",p1->id, p1->name, p1->price, p1->quantity);
            }
            else printf("Khong tim thay\n");
        }
        else if (choice == 2){
            gsl_t list = gsl_create(gsl_free_bill); /*Hàm gsl_free_bill ở trên dùng để truyền vào để tạo gsl*/
            while(strcmp(line, "STOP") != 0){
                printf("Nhap ID: ");
                cgetline(&line,0,stdin);
                remove_tail_lf(line);
                if(strcmp(line, "STOP") == 0) continue; /*Nếu gặp line == STOP, nó quay lại vòng lặp while, check điều kiện k đúng => dừng vòng lặp*/
                gtype *value = hmap_value(map, gtype_s(line));
                if(value){
                    hang tmp = value->v;
                    printf("Nhap so luong: ");
                    scanf("%d",&quantity);
                    clear_stdin();
                    if(quantity <= 0 || quantity >= tmp->quantity){
                        printf("So hang khong hop le\n");
                    }
                    else{
                        bill pp = malloc(sizeof(struct bill_t));
                        pp->name = strdup(tmp->name);
                        pp->price = tmp->price;
                        pp->quantity = quantity;
                        pp->total = quantity * tmp->price;
                        gsl_push_back(list, gtype_v(pp)); /*Truyền pp vào gsl, cú pháp theo khuôn mẫu của thầy*/
                    }
                }
                else printf("Khong co mat hang\n");
            }
            idx = 1;
            double tong = 0;
            printf("Cty ABC\n");
            printf("Don hang ban le\n");
            printf("%s %30s %30s %30s %30s\n", "STT", "Mat hang", "Don gia", "So luong", "Thanh tien");
            gsl_traverse(cur,list){
                bill tmp = cur->v;
                printf("%d %30s %30lf %30d %30lf\n",idx, tmp->name, tmp->price, tmp->quantity, tmp->total);
                idx++;
                tong += tmp->total;
            }
            printf("Tong tien: %lf\n",tong);
        }
    }
    /*Tôi chưa thể free hết, các bạn giúp tôi*/
    free(line);
    for(int i=0; i<n; i++){
        free(h[i]);
    }
    free(h);
    hmap_free(map);
    fclose(f);
}