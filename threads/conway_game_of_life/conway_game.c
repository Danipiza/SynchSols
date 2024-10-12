#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>

// sudo apt-get install libsdl2-dev
#include <SDL2/SDL.h>



/*
// COMPILE
    gcc -pthread conway_game.c -lSDL2

// EXECUTE
    ./a.out

Alive cells =1, 
Death cells =0,

Rules (executed per iteration):
    Alive: Each cell with 2 or 3 neighbors alive, remains alive.
            otherwise it dies. 
    Death: Each cell with 3 neigbors alive, reborns. 
            otherwise remains dead.

Neighbors are the adjacent cells, in vertical, horizontal and diagonal axis.
*/ 

#define NUM_THREADS 4



typedef struct {
    int id;
    int bounds[2]; 
} ThreadArgs;


int mX[8]={-1,-1,-1,0,0,1,1,1};
int mY[8]={-1,0,1,-1,1,-1,0,1};


int** m1;
int** m2;

int n, m;

// Function to draw the matrix in the GUI
void draw_matrix(SDL_Renderer *renderer, int** mat, int rows, int cols, double cell_size) {
    for (int i=0;i<rows;i++) {
        for (int j=0;j<cols;j++) {
            SDL_Rect cell;
            cell.x =j*cell_size;
            cell.y =i*cell_size;
            cell.w =cell_size;
            cell.h =cell_size;

            if (mat[i][j]==1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black for 1
            } 
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White for 0
            }


            SDL_RenderFillRect(renderer, &cell);
        }
    }
}


void init_random_matrix(int n, int m){
    srand(time(NULL));

    
    m1       =(int**)malloc(n*sizeof(int*));
    m2       =(int**)malloc(n*sizeof(int*));
    
    for(int i=0;i<n;i++) {
        m1[i]       =(int*)malloc(m*sizeof(int));
        m2[i]       =(int*)malloc(m*sizeof(int));
    }

       
    int num;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            num=(rand()%2);
            m1[i][j]=num;
        }
    }
}

void init_matrix(int n, int m/*, int** mat*/){
    srand(time(NULL));

    m1       =(int**)malloc(n*sizeof(int*));
    m2       =(int**)malloc(n*sizeof(int*));
    
    for(int i=0;i<n;i++) {
        m1[i]       =(int*)malloc(m*sizeof(int));
        m2[i]       =(int*)malloc(m*sizeof(int));
    }

    
    int aux[16]={0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1};
    int count=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){ 
            m1[i][j]=aux[count++];
        }
    }
}

void print_matrix(int** mat, int n, int m){
    for(int i=0;i<n;i++){
        for(int j=0;j<m-1;j++){
            printf("%d ",mat[i][j]);
        }
        printf("%d\n",mat[i][m-1]);
    }
}





void* func_1(void* arg){
    ThreadArgs* args=(ThreadArgs*) arg;    

    int count=0;
    int x, y;
    

    for(int i=args->bounds[0];i<args->bounds[1];i++){
        
        for(int j=0;j<m;j++){
            
            count=0;
            for(int k=0;k<8;k++) {                
                x=i+mX[k]; y=j+mY[k];                               
                if(x>=0&&x<n&&y>=0&&y<m&&
                    m1[x][y]==1) count++;                
            }
            

            if(m1[i][j]==0){ // dead
                if(count==3) m2[i][j]=1;
                else m2[i][j]=0;
            }
            else { // alive
                if(count==2 || count==3) m2[i][j]=1;
                else m2[i][j]=0;
            }
        }
    }

    
    
    

    free(args);        
    return NULL;
}

void* func_2(void* arg){
    ThreadArgs* args=(ThreadArgs*) arg;    

    int count=0;
    int x, y;
    

    for(int i=args->bounds[0];i<args->bounds[1];i++){
        
        for(int j=0;j<m;j++){
            
            count=0;
            for(int k=0;k<8;k++) {                
                x=i+mX[k]; y=j+mY[k];                               
                if(x>=0&&x<n&&y>=0&&y<m&&
                    m2[x][y]==1) count++;                
            }
            

            if(m2[i][j]==0){ // dead
                if(count==3) m1[i][j]=1;
                else m1[i][j]=0;
            }
            else { // alive
                if(count==2 || count==3) m1[i][j]=1;
                else m1[i][j]=0;
            }
        }
    }

    
    

    free(args);        
    return NULL;
}

int main(int argc, char** argv){
    pthread_t threads[NUM_THREADS];
    // 1080*1080=1.166.400
    // 1600*x=729
    // sqrt(729)=27
    // 25 -> 1000*1000 = 1.000.000

    int mat_size=100;
    int end=0;
    int div=mat_size/NUM_THREADS;

    double aux=1000000/(mat_size*mat_size);
    double cell_size=sqrt(aux);
    printf("celsize: %f\n",cell_size);

    n=m=mat_size;
    init_random_matrix(mat_size, mat_size);

    double window_height =n*cell_size;
    double window_width  =m*cell_size;
    
    


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Matrix GUI", 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    void* function=func_1;
    int tick=0;

    SDL_Event event;
    

    while(!end){

        while (SDL_PollEvent(&event)) {
            if (event.type==SDL_QUIT) end=1;
        }

        for(int i=0;i<NUM_THREADS;i++){
            ThreadArgs *args=(ThreadArgs*) malloc(sizeof(ThreadArgs));

            args->id=i;
            args->bounds[0]=div*i;
            args->bounds[1]=div*i+div;
            
            if(pthread_create(&threads[i], NULL, function, (void*) args)){
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
        

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);  // Clear the screen
        
        printf("Epoch: %d\n", tick);

        tick++;
        if(tick%2==0) {            
            /*printf("New mat1:\n");
            print_matrix(m1,mat_size, mat_size);
            printf("\n");*/            

            draw_matrix(renderer, m1, n, m, cell_size);           

            function=func_1;
        }
        else {
            /*printf("new mat2:\n");
            print_matrix(m2, mat_size, mat_size);
            printf("\n");*/           

            draw_matrix(renderer, m2, n, m, cell_size);       

            function=func_2;        
        }   
        SDL_RenderPresent(renderer);  // Present the updated screen

        //sleep(1);  
        // Delay a bit to prevent high CPU usage
        SDL_Delay(10);   
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    /*
    printf("Original:\n");
    print_matrix(mOriginal, mat_size, mat_size);
    printf("Original:\n");
    print_matrix(m1, mat_size,mat_size);
    */
    
    return 0;
}

