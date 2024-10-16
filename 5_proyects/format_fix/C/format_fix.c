#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <dirent.h>

#include <fcntl.h>
#include <string.h>

#define MAX_BUFF_SIZE 512
#define MAX_FILES 1028

typedef struct{
    int id;
    int bounds[2];
} ThreadArgs;

char dir_path[MAX_BUFF_SIZE];
char** file_names;



/*
Function to replace four consecutive spaces with a tab

Args:
    buffer (char*) : Chunk of characters to read.
    length (int)   : Length of the chunk.

void replace_spaces(char* buffer, int length) {
    char ret[MAX_BUFF_SIZE];
    
    int i=0, j=0;

    while (i<length-4) {
        
        // -- 4 spaces consecutives? -------------------------------------------
        if (buffer[i]  ==' ' && buffer[i+1]==' ' && 
            buffer[i+2]==' ' && buffer[i+3]==' ') {
            ret[j++]='\t'; 
            i+=4;           
        } 
        else ret[j++] = buffer[i++];         
    }

    // copy the modified string back to the original buffer
    memcpy(buffer, ret, j);
}
*/


void replace_spaces(char *line) {
    char result[MAX_BUFF_SIZE];
    int i=0, j=0;
    
    while (line[i]!='\0') {
        // Check if the next four characters are spaces
        if (line  [i]==' ' && line[i+1]==' ' && 
            line[i+2]==' ' && line[i+3]==' ') {
            result[j++] = '\t'; // Replace with a tab
            i += 4;             // Skip the four spaces
        } 
        else result[j++] = line[i++]; // Copy the character if no match
    }
    
    result[j]='\0'; // Null-terminate the result string
    strcpy(line, result); // Copy the modified line back to the original line
}

/*
Function to re-format a file.

Args:
    id (int)            : Thread ID.
    input_file (char*)  : Input file name.
    output_file (char*) : Output file name.

void format_file(int id, char* input_file, char* output_file){

    
    // -- reading --------------------------------------------------------------
    int input_fd=open(input_file, O_RDONLY);
    if (input_fd<0) {
        perror("Error while opening the input file");
        exit(EXIT_FAILURE);
    }

    
    // -- writing --------------------------------------------------------------
    // create if it doesn't exist, otherwise truncate it
    int output_fd=open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd<0) {
        perror("Error while opening the output file");
        close(input_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFF_SIZE];
    ssize_t bytes_read;

    
    // -- read the input file. chunk of 512 per iterarion ----------------------    
    while ((bytes_read = read(input_fd, buffer, MAX_BUFF_SIZE)) > 0) {
        replace_spaces(buffer, bytes_read); 
        
        // write the modified data to the output file
        write(output_fd, buffer, bytes_read); 
    }

    if (bytes_read<0) perror("Error reading from input file");

    // close file descriptors
    close(input_fd);
    close(output_fd);
}
*/

/*
Function to re-format a file.

Args:
    id (int)            : Thread ID.
    input_file (char*)  : Input file name.
    output_file (char*) : Output file name.
*/
void format_file(int id, char* input_file, char* output_file){

    
    FILE *input_fp  =fopen(input_file, "r");
    FILE *output_fp =fopen(output_file, "w");

    // -- reading --------------------------------------------------------------
    if (input_fp==NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // -- writing --------------------------------------------------------------
    if (output_fp==NULL) {
        perror("Error opening output file");
        fclose(input_fp);
        exit(EXIT_FAILURE);
    }
   
    

    char buffer[MAX_BUFF_SIZE];
    ssize_t bytes_read;

    
    
    char line[MAX_BUFF_SIZE];

        
    // -- read each row of the input file. -------------------------------------    
    while (fgets(line, sizeof(line), input_fp) != NULL) {
        replace_spaces(line); 
        
        // write the modified line to the output file
        fputs(line, output_fp);        
    }

    
    // -- close ----------------------------------------------------------------
    fclose(input_fp);
    fclose(output_fp);    
}

/*
Thread function.

Args (ThreadArgs):
    id (int)        : Thread ID.
    bounds (int[2]) : Divided files to process.
*/
void* func(void* arg){
    ThreadArgs* args=(ThreadArgs*) arg;
    
    int n;
    for(int i=args->bounds[0];i<args->bounds[1];i++){        
        
        n=strlen(file_names[i]);
        char* output=(char*)malloc((n+1)*sizeof(char));
        output[0]='z';
        for(int j=1;j<n+1;j++) output[j]=file_names[i][j-1];
        
        format_file(args->id, file_names[i], output);
        free(output);
    }
    

    return NULL;
}

int main(int argc, char** argv){
    // init array of strings with null values.
    file_names=(char**)calloc(MAX_FILES,sizeof(char*));
    
    int num_threads=3;
    int num_files  =0;


    // -- reading --------------------------------------------------------------
    int  file_ext_size=5;
    char file_extension[5]=".java";  

    // get directory path
    if (getcwd(dir_path, sizeof(dir_path))!=NULL) {
        printf("Current working directory: %s\n", dir_path);
    } 
    else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }

    int count=0; // number of "good" files
    int k, n;

    DIR* directory=opendir(dir_path);
    struct dirent* rd=readdir(directory);
    while(rd!=NULL){
        n=strlen(rd->d_name);

        //printf("Name: %s\tsize:%d\n", rd->d_name, n);
        
        for(k=0;k<file_ext_size;k++){
            /*printf("%c - %c\n",rd->d_name[n-k-1], 
                file_extension[file_ext_size-k-1]);*/
            if(rd->d_name[n-k-1]!=
               file_extension[file_ext_size-k-1]) break;
        }

        if(k==file_ext_size) file_names[count++]=rd->d_name;

        rd=readdir(directory);
    }

    /*for(int i=0;i<count;i++){
        printf("(%d) Name: %s\n",i,file_names[i]);
    }*/
    printf("GOOD FILES: %d\n", count);

    
    // -- executing threads ----------------------------------------------------

    int div=count/num_threads;
    pthread_t threads[num_threads];
    for(int i=0;i<num_threads;i++){
        
        // -- thread arguments -------------------------------------------------
        ThreadArgs* args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->id=i;
        args->bounds[0]=i*div;
        args->bounds[1]=i*div+div;

        if(pthread_create(&threads[i], NULL, &func, (void*) args)){
            perror("Error while creating a thread");
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0;i<num_threads;i++){
        if(pthread_join(threads[i], NULL)){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}