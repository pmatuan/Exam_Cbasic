#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#include "io.h"
typedef struct hang_t{
    char *id;
    char *company;
}*hang;
typedef struct count{
    int start;
    int end;
    int dem;
}*count;
int cmp_id(const void *a, const void *b){
    char *p1 = (*(hang*)a)->id;
    char *p2 = (*(hang*)b)->id;
    return strcmp(p1,p2);
}
int cmp_company(const void *a, const void *b){
    char *p1 = (*(hang*)a)->company;
    char *p2 = (*(hang*)b)->company;
    return strcmp(p1,p2);
}
int cmp_dem(const void *a, const void *b){
    int x = (*(count*)a)->dem;
    int y = (*(count*)b)->dem;
    return y - x;
}
int main(int argc, char *argv[]){
    clock_t start = clock();
    int n, idx = 0;
    char *line = NULL, *id = NULL, *company = NULL, *tmp = NULL;
    char mode[2];
    FILE *f = fopen(argv[argc-1], "r");
    cgetline(&line,0,f);
    n = atoi(line);
    hang *list = calloc(n,sizeof(hang));
    while(cgetline(&line,0,f)){
        list[idx] = malloc(sizeof(struct hang_t));
        id = strtok(line, " \n");
        company = strtok(NULL, " \n");
        list[idx]->id = strdup(id);
        list[idx]->company = strdup(company);
        idx++;
    }
    sscanf(argv[1],"%s",mode);
    if(!strcmp(mode,"f1")){
        tmp = argv[2];
        hang *h_tmp = calloc(n,sizeof(hang));
        idx = 0;
        for(int i=0; i<n; i++){
            if(!strcmp(tmp,list[i]->company)){
                h_tmp[idx] = malloc(sizeof(struct hang_t));
                h_tmp[idx]->id = list[i]->id;
                idx++;
            }
        }
        qsort(h_tmp,idx,sizeof(h_tmp),cmp_id);
        for(int i=0; i<idx; i++){
            printf("%s ",h_tmp[i]->id);
        }
        free(h_tmp);
    }
    else if(!strcmp(mode, "f2")){
        tmp = argv[2];
        hang *h_tmp = calloc(n,sizeof(hang));
        idx = 0;
        for(int i=0; i<n; i++){
            if(!strcmp(tmp,list[i]->id)){
                h_tmp[idx] = malloc(sizeof(struct hang_t));
                h_tmp[idx]->company = list[i]->company;
                idx++;
            }
        }
        qsort(h_tmp,idx,sizeof(h_tmp),cmp_company);
        for(int i=0; i<idx; i++){
            printf("%s ",h_tmp[i]->company);
        }
        free(h_tmp);
    }
    else if(!strcmp(mode, "f3")){
        qsort(list,n,sizeof(list),cmp_id);
        int start = 0, end = 0, i = 0;
        idx = 0;
        count *ct = calloc(n,sizeof(count)); 
        while(i < n){
            start = i;
            while(i < n && !strcmp(list[start]->id, list[i]->id)){
                i++;
            }
            if(start == i) i++;
            end = i;
            ct[idx] = malloc(sizeof(struct count));
            ct[idx]->start = start;
            ct[idx]->end = end;
            ct[idx]->dem = end-start;
            idx++;
        }
        qsort(ct, idx, sizeof(count),cmp_dem);
        hang *h_tmp = calloc(n,sizeof(hang));
        idx = 0;
        for(int i = ct[0]->start; i<ct[0]->end; i++){
            h_tmp[idx] = malloc(sizeof(struct hang_t));
            h_tmp[idx]->company = list[i]->company;
            idx++;
        }
        qsort(h_tmp,idx,sizeof(h_tmp),cmp_company);
        printf("%s %d\n",list[ct[0]->start]->id,ct[0]->dem);
        for(int i=0; i<idx; i++){
            printf("%s ",h_tmp[i]->company);
        }
        free(h_tmp);
        free(ct);
    }
    printf("\n");
    fclose(f);
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Thoi gian chay: %lf\n", cpu_time_used);
    free(line);
    free(id);
    free(company);
    free(tmp);
    free(list);
    return 0;
}