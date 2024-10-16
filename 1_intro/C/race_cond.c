#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int x=0;
pthread_mutex_t mut;

/*
ASSEMBLY CODE:

	.file	"race_cond.c"
	.text
	.globl	x					; global variable
	.bss
	.align 4
	.type	x, @object
	.size	x, 4
x:
	.zero	4					; x = 0
	.text
	.globl	func
	.type	func, @function
func: 
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2                     ; THIS PART OF THE CODE IS THE IMPORTANT ONE
.L3:							; for loop
	movl	$1, %edi
	call	usleep@PLT			; sleep
	movl	x(%rip), %eax		; get x from memory	
	addl	$1, %eax			; add 1 to x
	movl	%eax, x(%rip)		; moves the new x value to the main memory	
	addl	$1, -4(%rbp)		; increment loop variable
.L2:
	cmpl	$9, -4(%rbp)		; jump condition
	jle	.L3
    ...                         ; MORE CODE
*/

void* no_mutex_func(void* args){

    for(int i=0;i<10;i++){
        usleep(1); // sleep for 1 microsecond
        x++;
    }
}

void* with_mutex_func(void* args){

    for(int i=0;i<10;i++){
        usleep(1); // sleep for 1 microsecond
        pthread_mutex_lock(&mut);
        x++;
        pthread_mutex_unlock(&mut);
    }
}

void no_mutex(){
    int num_threads=100;
    
    pthread_t t[num_threads];

    for(int j=0;j<num_threads;j++){
        if(pthread_create(&t[j], NULL, no_mutex_func, NULL)!=0){            
            perror("Error while creating a thread\n");
            exit(EXIT_FAILURE);
        }   
    }
    
    for(int j=0;j<num_threads;j++){
        if(pthread_join(t[j],NULL)!=0){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }   
    }
    

    
    printf("x: %d\n",x);
}


void with_mutex() {
    int num_threads=100;

    pthread_mutex_init(&mut, NULL);
    
    pthread_t t[num_threads];

    for(int j=0;j<num_threads;j++){
        if(pthread_create(&t[j], NULL, with_mutex_func, NULL)!=0){            
            perror("Error while creating a thread\n");
            exit(EXIT_FAILURE);
        }   
    }
    
    for(int j=0;j<num_threads;j++){
        if(pthread_join(t[j],NULL)!=0){
            perror("Error while waiting for a thread");
            exit(EXIT_FAILURE);
        }   
    }
    

    
    printf("x: %d\n",x);
    
    pthread_mutex_destroy(&mut);
}

int main(int argc, char** argv){
    //no_mutex();
    with_mutex();

    return 0;
}