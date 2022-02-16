#include "all.c"
#include "io.h"
#define strequal(s1,s2) (strcmp(s1,s2) == 0)
typedef char string[21];
/*Dùng biến toàn cục để đỡ truyền thêm vào các hàm*/
string *id,*company; // id: mã số mặt hàng, company: công ty cung cấp
int n;



/*In ra string của vector*/
void print_gvec(gvec_t v){
    gvec_traverse(cur,v){
        printf("%s ",cur->s);
    }
}
/********************************/



/*Chức năng 1*/
/*Qua thực nghiệm, gvec có vẻ nhanh hơn cách làm bằng mảng*/
void f1(const char *p){
    gvec_t vec = gvec_create(10, NULL);
    /*Có thể khởi tạo giá trị bất kì cho vec, nếu ít thì mất thêm thời gian scale, nếu nhiều thì tốn bộ nhớ*/ 
    for(int i=0; i<n; ++i){
        if(strequal(p,company[i])){
            gvec_append(vec,gtype_s(id[i]));
            /*Thêm thành phần vào vector, lưu ý phải cho vào gtype_s*/
            /*gvec_append(vec,id[i]) => Báo lỗi*/
        }
    }
    gvec_qsort(vec,gtype_qsort_s);
    print_gvec(vec);
    gvec_free(vec);
}
/********************************/



/*Chức năng 2*/
/*Tương tự tính năng 1*/
void f2(const char *p){
    gvec_t vec = gvec_create(10, NULL);
    for(int i=0; i<n; ++i){
        if(strequal(p,id[i])){
            gvec_append(vec,gtype_s(company[i]));
        }
    }
    gvec_qsort(vec,gtype_qsort_s);
    print_gvec(vec);
    gvec_free(vec);
}
/********************************/



/*Chức năng 3*/
/*
    Ý tưởng: dùng hmap
    Cấu trúc map: mã số mặt hàng -> số lượng
    Ví dụ: hmap["h1"] = 3: số lượng mặt hàng h1 là 3
*/
void f3(){
    hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
    for(int i=0; i<n; ++i){
        hmap_ires res = hmap_insert(map,gtype_s(id[i]), gtype_l(1));
        /*
        Nếu chỉ insert vào hmap, chỉ cần câu lệnh
        hmap_insert(map,gtype_s(id[i]), gtype_l(1));
        là đủ, ở đây thêm phần res để kiểm tra đã tồn tại
        khóa đó trong map của mình chưa, nếu chưa thì inserted = 1,
        nếu đã tồn tại inserted = 0 để tăng giá trị của khóa lên
        Ví dụ: hmap["h1"] = 1, thêm vào 1 khóa h1 nữa thì giờ hmap["h1"] = 2
        */
        if(!res.inserted){
            ++(res.value->l);
        }
        /*
        Code tương đương, có thể dễ hiểu hơn
        if(!res.inserted){
            gtype *tmp = hmap_value(map,gtype_s(id[i])); // lấy giá trị của khóa 
            ++(tmp->l); // tăng giá trị của khóa lên 1 
        }
        */
    }
    /*
    Yêu cầu đề bài: đưa ra sản phẩm có nhiều nhà cung cấp nhất
    => bài toán trở thành: tìm khóa có giá trị lớn nhất trong map => duyệt tuần tự để tìm
    */
    long max = 0; //giá trị lớn nhất
    const char *maxelement; // lưu lại khóa có giá trị lớn nhất
    hmap_traverse(k,v,map){
        if(max < v->l){
            max = v->l;
            maxelement = k->s;
        }
    }
    /* Cho các company cung cấp sản phẩm maxelement vào vector */
    gvec_t vec = gvec_create(10, NULL);
    for(int i=0; i<n; ++i){
        if(strequal(maxelement,id[i])){
            gvec_append(vec,gtype_s(company[i]));
        }
    }
    gvec_qsort(vec,gtype_qsort_s);

    printf("So luong: %ld\n",max);
    print_gvec(vec);
    gvec_free(vec);
    hmap_free(map);
}



int main(int argc, char *argv[]){
    FILE *f = fopen(argv[argc-1], "r");
    fscanf(f, "%d", &n);
    id = malloc(n*sizeof(string));
    company = malloc(n*sizeof(string));
    for(int i=0; i<n; i++){
        fscanf(f, "%s %s",id[i],company[i]);
    }
    fclose(f);
    if(strequal("f1",argv[1])){
        f1(argv[2]);
    }
    else if(strequal("f2",argv[1])){
        f2(argv[2]);
    }
    else if(strequal("f3",argv[1])){
        f3();
    }
    printf("\n");
    free(id);
    free(company);
    return 0;
}