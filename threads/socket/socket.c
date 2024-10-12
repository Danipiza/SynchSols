#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>


#include <pthread.h>

#define BUF_SIZE 500

/*
// COMPILE:
    gcc -pthread socket.c

// EXECUTE
    ./a.out <ip> <port>

    Example:
    ./a.out :: 8080

    (in another terminal) IPv6
    nc ::1 8080
    <messages>

    (in another terminal) IPv4
    nc 127.0.0.1 8080
    <messages>

// FINISH: With a kill signal (Ctrl + C) in the server terminal.
*/


typedef struct{
    int clientsd;
    struct sockaddr_storage sa;
    socklen_t salen;
} ThreadArgs;

void* func(void* arg){
    ThreadArgs* args=(ThreadArgs*) arg;

    // -------------------------------------------------------------------------
    ssize_t bytes_read;
    char buf[BUF_SIZE];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int s=getnameinfo((struct sockaddr *) &args->sa,
                        args->salen, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    
    if (s!=0) fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    
    printf("Connected client IP: %s, Port: %d\n", host, service);
    
    while(1){
        bytes_read = recv(args->clientsd, buf, BUF_SIZE, 0);
        if (bytes_read<=0) break;               /* Ignore failed request */           

        
        printf("Message from %s-%s: %s", host, service, buf);
        
        // DONT send message back
        /*if (send(args->clientsd, buf, bytes_read, 0) != bytes_read)
            fprintf(stderr, "Error sending response\n");*/
    }
    printf("Conexion terminada\n");
}

int main(int argc, char *argv[]) {
    if (argc!=3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    struct addrinfo *ai, *rp;
    int sfd, s;
    struct sockaddr_storage sa;
    socklen_t salen;
    ssize_t nread;
    char buf[BUF_SIZE];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   =AF_UNSPEC;   // Allow IPv4 or IPv6 
    hints.ai_socktype =SOCK_STREAM; // Datagram socket 
    hints.ai_flags    =AI_PASSIVE;  // For wildcard IP address

    s=getaddrinfo(argv[1], argv[2], &hints, &ai);
    if (s!=0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp=ai; rp!=NULL; rp=rp->ai_next) {
        sfd=socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd==-1) continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen)==0)
            break;                  /* Success */

        close(sfd);
    }
   
    freeaddrinfo(ai);  // No longer needed 
    if (rp == NULL) {  // No address succeeded 
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    if(listen(sfd,16)==-1) { perror("listen()"); exit(EXIT_FAILURE); }

    int clientsd;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    

    while(1) {
        clientsd=accept(sfd, (struct sockaddr *) &sa, &salen);
        if(clientsd==-1) { perror("accept()"); continue; }
        

        
        // -- create a new thread for each client ------------------------------        
        ThreadArgs* args=(ThreadArgs*)malloc(sizeof(ThreadArgs));
        args->clientsd =clientsd;
        args->sa       =sa;
        args->salen    =salen;

        
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, func, (void *)args) != 0) {
            perror("Thread creation failed");
            continue;
        }

        
        // -- detach the thread ------------------------------------------------ 
        // cleans up automatically when it exits
        pthread_detach(client_thread);
    }

    return 0;
}