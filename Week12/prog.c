/*
    Bài tập tuần 12: Các đơn hàng có giá trị lớn nhất
    Làm theo hmap
*/
#include "all.c"
#include "io.h"
typedef char string[21];


char *line = NULL, *name = NULL;
string *id;
int n, m, price, quantity = 0, sl = 0, idx = 0;
gtype *v;


/*Lưu tên và giá tiền trong orders*/
typedef struct bill_t{
    char *name;
    int price;
}*bill;
#define pp(val) ((bill)(((gtype*)val)->v))

/*Hàm compare quicksort*/
int cmp(const void *a, const void *b){
    int m = (*(bill*)a)->price;
    int n = (*(bill*)b)->price;
    return n-m;
}

/*Hàm compare quicksort gvec*/
int gtype_qsort_bill(const void *v1, const void *v2){
    return pp(v2)->price - pp(v1)->price;
}

int main(int argc, char *argv[]){
    clock_t start = clock();
    /*Khởi tạo bảng băm lưu mã số và giá tiền trong products*/
    hmap_t products = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
    FILE *f = fopen(argv[1], "r");
    cgetline(&line,0,f);
    n = atoi(line);
    id = malloc(n*sizeof(string));
    for(int i=0; i<n; i++){
        fscanf(f,"%s %d", id[i], &price);
        /*Thêm vào bảng băm, với khóa là mã số và giá trị là giá tiền*/
        hmap_insert(products, gtype_s(id[i]), gtype_l(price));
    }
    fclose(f);
    FILE *f1 = fopen(argv[2], "r");
    cgetline(&line,0,f);
    m = atoi(line);
    /*Có thể sử dụng gvec, nhưng ở đây không thấy quá nhiều lợi ích nên sử dụng mảng cho đơn giản*/



    /*Cách 1: mảng*/
    bill *orders = malloc(m * sizeof(bill));
    while(cgetline(&line,0,f)){
        orders[idx] = malloc(sizeof(struct bill_t));
        price = 0; // Do mỗi lần chúng ta add price vào trong mảng, nên với mỗi lần đọc dòng, ta reset price về 0 để cộng chính xác
        name = strtok(line, " \n");
        quantity = atoi(strtok(NULL," \n")); // số lượng mặt hàng có trong đơn hàng
        /*Duyệt trong số lượng mặt hàng*/
        for(int i=0; i<quantity; i++){
            v = hmap_value(products, gtype_s(strtok(NULL," \n")));
            sl = atoi(strtok(NULL," \n"));
            price += v->l * sl; // lý do phải reset price về 0 
        }
        orders[idx]->name = strdup(name);
        orders[idx]->price = price;
        idx++;
    }
    qsort(orders, m, sizeof(orders), cmp);
    /*Lấy 3 phần tử lớn nhất*/
    for(int i=0; i<3; i++){
        printf("%s %d\n", orders[i]->name, orders[i]->price);
    }



    /*Cách 2: Gvec*/
    // gvec_t vec = gvec_create(10, NULL);
    // bill *orders = malloc(m * sizeof(bill));
    // while(cgetline(&line,0,f)){
    //     orders[idx] = malloc(sizeof(struct bill_t));
    //     price = 0; // Do mỗi lần chúng ta add price vào trong mảng, nên với mỗi lần đọc dòng, ta reset price về 0 để cộng chính xác
    //     name = strtok(line, " \n");
    //     quantity = atoi(strtok(NULL," \n")); // số lượng mặt hàng có trong đơn hàng
    //     /*Duyệt trong số lượng mặt hàng*/
    //     for(int i=0; i<quantity; i++){
    //         v = hmap_value(products, gtype_s(strtok(NULL," \n")));
    //         sl = atoi(strtok(NULL," \n"));
    //         price += v->l * sl; // lý do phải reset price về 0 
    //     }
    //     orders[idx]->name = strdup(name);
    //     orders[idx]->price = price;
    //     gvec_append(vec, gtype_v(orders[idx]));
    //     idx++;
    // }
    // gvec_qsort(vec, gtype_qsort_bill);
    // idx = 0;
    // gvec_traverse(cur, vec){
    //     if(idx >=3) break;
    //     bill tp = cur->v;
    //     printf("%s %d\n", tp->name, tp->price);
    //     idx++;
    // }

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Thoi gian chay: %lf\n", cpu_time_used);
    fclose(f1);
    // chưa free sạch, mọi người giúp mình 
    hmap_free(products);
    free(id);
    free(line);
    return 0;
}