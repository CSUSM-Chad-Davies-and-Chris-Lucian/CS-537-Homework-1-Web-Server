/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *cliSvr(void *arg)
{

    int   n, sockfd;
    char  buffer[256];

    bzero(buffer,256);

    sockfd = *(int *)arg;

    /* Wait for request from client */
    n = read(sockfd,buffer,255);
    if (n < 0) {
        fprintf(stderr, "Error reading from socket, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }

    printf("Server:: Here is the message: %s\n",buffer);

    /* Send response back to the client */
    n = write(sockfd,"I got your message",18);
    if (n < 0)  {
        fprintf(stderr, "Error writing to socket, errno = %d (%s)\n",
                errno, strerror(errno));
        close(sockfd);
        return NULL;
    }

    close(sockfd);
    return NULL;

}


int main(int argc, char *argv[])
{
     int sockfd, clisockfd, port;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     pthread_t  tid;

     if (argc < 2) {
         fprintf(stderr,"Usage: %s <port>\n", argv[0]);
         exit(1);
     }

     /* Open a TCP socket connection */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
     }

     bzero((char *) &serv_addr, sizeof(serv_addr));
     port = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
         fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                 errno, strerror(errno));
         return -1;
     }

     /* Setup passive listening socket for client connections */
     listen(sockfd, 5);

     /* Wait for incoming socket connection requests */
     while (1) {
         clilen = sizeof(cli_addr);
         clisockfd = accept(sockfd,
                            (struct sockaddr *) &cli_addr,
                            &clilen);

         if (clisockfd < 0) {
             fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                     errno, strerror(errno));
             break;
         }

         /* Create thread for client requests/responses */
         pthread_create(&tid, NULL, (void *)&cliSvr, (void *)&clisockfd);
     }

     close(sockfd);

     return 0;
}