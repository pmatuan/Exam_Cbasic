#include "all.c"
#include "ext/io.h"
typedef char string[21];
typedef struct pair_t{
    string id;
    string company;
}*pair;
int n, idx = 0;
int main(int argc, char *argv[]){
    FILE *f = fopen(argv[argc-1], "r");
    fscanf(f, "%d", &n);
    fgetc(f);
    pair *p = calloc(n, sizeof(pair));
    for(int i=0; i<n; i++){
        p[i] = malloc(sizeof(struct pair_t));
        fscanf(f, "%s %s", p[i]->id, p[i]->company);
    }
    if(!strcmp(argv[1], "f1")){
        gvec_t vec = gvec_create(10, NULL);
        for(int i=0; i<n; i++){
            if(!strcmp(p[i]->company, argv[2])){
                gvec_append(vec, gtype_s(p[i]->id));
            }
        }
        gvec_qsort(vec, gtype_qsort_s);
        gvec_traverse(cur, vec){
            printf("%s ", cur->s);
        }
        printf("\n");
        gvec_free(vec);
    }
    else if(!strcmp(argv[1], "f2")){
        gvec_t vec = gvec_create(10, NULL);
        for(int i=0; i<n; i++){
            if(!strcmp(p[i]->id, argv[2])){
                gvec_append(vec, gtype_s(p[i]->company));
            }
        }
        gvec_qsort(vec, gtype_qsort_s);
        gvec_traverse(cur, vec){
            printf("%s ", cur->s);
        }
        printf("\n");
        gvec_free(vec);
    }
    else if(!strcmp(argv[1], "f3")){
        hmap_t map = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
        for(int i=0; i<n; i++){
            hmap_ires res = hmap_insert(map, gtype_s(p[i]->id), gtype_l(1));
            if(!res.inserted) res.value->l++;
        }
        int max = -1;
        char *maxelement;
        hmap_traverse(k, v, map){
            if(v->l > max){
                max = v->l;
                maxelement = k->s;
            }
        }
        printf("%s %d\n", maxelement, max);
        gvec_t vec = gvec_create(10, NULL);
        for(int i=0; i<n; i++){
            if(!strcmp(p[i]->id, maxelement)){
                gvec_append(vec, gtype_s(p[i]->company));
            }
        }
        gvec_qsort(vec, gtype_qsort_s);
        gvec_traverse(cur, vec){
            printf("%s ", cur->s);
        }
        printf("\n");
        gvec_free(vec);
        hmap_free(map);
    }
    for(int i=0; i<n; i++){
        free(p[i]);
    }
    free(p);
    fclose(f);
}