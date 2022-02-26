#include "all.c"
struct prices {
  double open, close;
};
rbm_t db;
void parse_input() {
  FILE *inp = fopen("data.txt", "r");
  int n;
  fscanf(inp, "%d", &n);
  char name[20];
  double o, c;
  db = rbm_create(gtype_cmp_s, gtype_free_s, gtype_free_gvec);
  for (int i = 0; i < n * 10; ++i) {
    fscanf(inp, "%s%lf%lf", name, &o, &c);
    gtype *tmp = rbm_value(db, gtype_s(name));
    struct prices *p = malloc(sizeof(struct prices));
    p->open = o;
    p->close = c;
    if (tmp) {
      gvec_append(tmp->gvec, gtype_v(p));
    } else {
      gvec_t vec = gvec_create(10, gtype_free_v);
      gvec_append(vec, gtype_v(p));
      rbm_put(db, gtype_s(strdup(name)), gtype_gvec(vec));
    }
  }
  fclose(inp);
}
int menu() {
  printf("1. Đọc tệp.\n");
  printf("2. Tìm mã.\n");
  printf("3. Tìm mã tăng. \n");
  printf("4. Số ngày tăng lớn nhất.\n");
  printf("5. Thoát.\n");
  int k = 0;
  printf("Lựa chọn của bạn [1..5]: ");
  scanf("%d", &k);
  while (getchar() != '\n') ;
  return k;
}
void f1() {
  rbm_traverse(k, v, db) {
    gvec_t vec = v->gvec;
    double total = 0;
    gvec_traverse(cur, vec) {
      struct prices* pp = cur->v;
      total += pp->close - pp->open;
    }
    total /= gvec_size(vec);
    printf("%s: %.3f\n", k->s, total);
  }
}
void f2() {
  char buff[20];
  printf("Mời nhập 1 mã: ");
  scanf("%s", buff);
  gtype *tmp = rbm_value(db, gtype_s(buff));
  if (!tmp) {
    printf("Mã %s không tồn tại.\n", buff);
    return;
  }
  double max = -1, min = -1;
  gvec_traverse(cur, tmp->gvec) {
    struct prices *pp = cur->v;
    if (max < pp->close || max < 0) {
      max = pp->close;
    }
    if (min > pp->close || min < 0) {
      min = pp->close;
    }
  }
  printf("Mã %s có giá đóng cửa cao nhất = %.3f và thấp nhất = %.3f\n",
          buff, max, min);
}
void f3() {
  rbm_traverse(k, v, db) {
    gtype *a = gvec_arr(v->gvec);
    struct prices *pp1 = a[0].v,
           *pp2 = a[1].v;
    if (pp1->close > pp1->open && pp2->close > pp2->open) {
      printf(" %s", k->s);
    }
  }
  printf("\n");
}
void f4() {
  long max = -1;
  rbm_t res = rbm_create(gtype_cmp_s, NULL, NULL);
  rbm_traverse(k, v, db) {
    gtype *tmp = rbm_insert(res, gtype_s(k->s), gtype_l(0)).value;
    gvec_traverse(cur, v->gvec) {
      struct prices *pp = cur->v;
      if (pp->close > pp->open) {
        ++tmp->l;
      }
    }
    if (tmp->l > max) {
      max = tmp->l;
    }
  }
  rbm_traverse(k, v, res) {
    if (v->l == max) {
      printf(" %s", k->s);
    }
  }
  printf(" có số ngày tăng = %ld\n", max);
  rbm_free(res);
}
int main() {
  parse_input();
  for (;;) {
    switch (menu()) {
      case 1: f1(); break;
      case 2: f2(); break;
      case 3: f3(); break;
      case 4: f4(); break;
      case 5: printf("Nguyễn Bá Ngọc\n");
              goto end;
      default: printf("Unknown command.");
    }
  }
  end:
  rbm_free(db);
  return 0;
}