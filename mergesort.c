#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<limits.h>
#define NOTHREADS 2
#define MAX 130000
/*
gcc -ggdb -lpthread parallel-mergesort.c 
NOTE: 
The mergesort boils downs to this.. 
Given two sorted array's how do we merge this?
We need a new array to hold the result of merging
otherwise it is not possible to do it using array, 
so we may need a linked list
*/

//int a[] = {10, 8, 5, 2, 3, 6, 7, 1, 4, 9};
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
    int i = 0;
    int count = 0;
    NODE m;
    FILE *fp;
    FILE *rp;
    char *token;
    char str[MAX];
    fp = fopen(argv[1],"r");
    while(fgets(str,MAX,fp) != NULL){
        a=(int *)malloc(sizeof(int)*100000);
        token = strtok(str, ' ');
        while( token != NULL ){
            //printf( "%s\n", token );   
            a[count++] = atoi(token);
            token = strtok(NULL, ' ');
        }
        m.i = 0;
        m.j = count - 1;
        pthread_t tid;
        pthread_create(&tid, NULL, mergesort, &m);
        pthread_join(tid, NULL);
        rp = fopen(argv[2],"a");
        for (i = 0; i < count; i++){
            fprintf(rp, "%d ", a[i]);
            printf ("%d ", a[i]);
        }
        printf ("\n");

        free(a);
        count = 0;
    }
    fclose(fp);
    fclose(rp);
    // pthread_exit(NULL);
    return 0;
}
