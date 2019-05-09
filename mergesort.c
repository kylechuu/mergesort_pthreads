#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
//#define NOTHREADS 2
#define MAX 130000
int *a;

typedef struct node{
    int i;
    int j;
} NODE;

void merge(int i, int j){
    int mid = (i+j)/2;
    int ai = i;
    int bi = mid+1;

    int newa[j-i+1], newai = 0;

    while(ai <= mid && bi <= j) {
            if (a[ai] > a[bi])
                    newa[newai++] = a[bi++];
            else                    
                    newa[newai++] = a[ai++];
    }

    while(ai <= mid) {
            newa[newai++] = a[ai++];
    }

    while(bi <= j) {
            newa[newai++] = a[bi++];
    }

    for (ai = 0; ai < (j-i+1) ; ai++)
            a[i+ai] = newa[ai];
}

void * mergesort(void *a){
    NODE *p = (NODE *)a;
    NODE n1, n2;
    int mid = (p->i+p->j)/2;
    pthread_t tid1, tid2;
    int ret;

    n1.i = p->i;
    n1.j = mid;

    n2.i = mid+1;
    n2.j = p->j;

    if (p->i >= p->j) return 0;

    pthread_create(&tid1, NULL, mergesort, &n1);
    pthread_create(&tid2, NULL, mergesort, &n2);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge(p->i, p->j);
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    clock_t start_time, end_time;
    float total_time = 0;
    start_time = clock();
    int i = 0;
    int count = 0;
    NODE m;
    FILE *fp;
    FILE *rp;
    char *token;
    char *dilem = " "; 
    char str[MAX];
    fp = fopen(argv[1],"r");
    while(fgets(str,MAX,fp) != NULL){
        a=(int *)malloc(sizeof(int)*100000);
        token = strtok(str, dilem);
        while( token != NULL ){
            //printf( "%s\n", token );   
            a[count++] = atoi(token);
            token = strtok(NULL, dilem);
        }
        m.i = 0;
        m.j = count - 1;
        pthread_t tid;
        pthread_create(&tid, NULL, mergesort, &m);
        pthread_join(tid, NULL);
        rp = fopen(argv[2],"a");
        for (i = 0; i < count; i++){ 
            if(i == count - 1)
                fprintf(rp, "%d", a[i]);
            else
                fprintf(rp, "%d ", a[i]);
            printf ("%d ", a[i]);
        }
        printf ("\n");
        fprintf(rp, "\n");
        free(a);
        count = 0;
        fclose(rp);
    }
    fclose(fp);
    end_time = clock();
    total_time = (float)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time: %f sec \n", total_time);
    return 0;
}
