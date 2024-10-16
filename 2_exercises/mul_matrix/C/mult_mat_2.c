#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

#define NUM_THREADS 4
#define MAT_SIZE 8



typedef struct {
    int id;    
    int row_bound[2];

    int** mA;
    int** mB;
    int** mRet;
} ThreadArgs;

void* func(void* arg){
    ThreadArgs* args=arg;
    
    int sum=0;
    for(int i=args->row_bound[0];i<args->row_bound[1];i++) {
        for(int j=0;j<MAT_SIZE;j++) {
            sum=0;
            for(int k=0;k<MAT_SIZE;k++) {
                sum+=args->mA[i][k]*args->mB[k][j];
            }

            args->mRet[i][j]=sum;
        }        
    }    
    printf("%d Llega\n",args->id);       
}

void init_mat(int n, int m,
    int*** mA, int*** mB,int*** mRet){
    srand(time(NULL));

    *mA=(int**)malloc(n*sizeof(int*));
    *mB=(int**)malloc(n*sizeof(int*));
    *mRet=(int**)malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        (*mA)[i]=(int*)malloc(m*sizeof(int));
        (*mB)[i]=(int*)malloc(m*sizeof(int));
        (*mRet)[i]=(int*)malloc(m*sizeof(int));
    }

    // random init for A and B
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            (*mA)[i][j]=(rand()%9)+1;
            (*mB)[i][j]=(rand()%9)+1;
        }
    }
}

void print_matrix(int** mat, int n, int m, char c){
    for(int i=0;i<n;i++){
        for(int j=0;j<m-1;j++){
            printf("%d%c", mat[i][j], c);
        }
        printf("%d\n", mat[i][m-1]);
    }
}


int main(int argc, char** argv) {    

    int** mA;
    int** mB;
    int** mRet;
    
    pthread_t threads[NUM_THREADS];
    int n=MAT_SIZE, m=MAT_SIZE;
    int div=MAT_SIZE/NUM_THREADS;
    init_mat(n, m, &mA, &mB, &mRet);
    
    
    print_matrix(mA, n, m, ' ');
    printf("\n");
    print_matrix(mB, n, m, ' ');

    
    for(int i=0;i<NUM_THREADS;i++){
        ThreadArgs* args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
        
        args->id=i;
        args->row_bound[0]=i*div;
        args->row_bound[1]=args->row_bound[0]+div;
        args->mA=mA;
        args->mB=mB;
        args->mRet=mRet;

        
        if(pthread_create(&threads[i], NULL, &func, (void*) args)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    
    
    for(int i=0;i<NUM_THREADS;i++){
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
    

    printf("Matrix A:\n");
    print_matrix(mA, n, m,' ');

    printf("Matrix B:\n");
    print_matrix(mB, n, m,' ');

    printf("Return matrix:\n");
    print_matrix(mRet, n, m,'\t');

    // -- free the memory ------------------------------------------------------
    for(int i=0;i<n;i++){
        free(mA[i]);
        free(mB[i]);
        free(mRet[i]);
    }
    free(mA); free(mB); free(mRet);

    return 0;
}