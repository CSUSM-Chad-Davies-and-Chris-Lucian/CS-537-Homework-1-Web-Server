#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{

    int    sockfd,  port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    /* Create the TCP socket for communication */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)  {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    /* Get the host name (i.e. the name or IP address of the server) */
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Error getting host, h_errno = %d (%s) \n",
                h_errno,hstrerror(h_errno));
        return -1;
    }

    /* Setup the socket for network comms */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
           server->h_length);

    /* Get the service name/port number */
    port = atoi(argv[2]);
    /* have to convert the port number to network format */
    serv_addr.sin_port = htons(port);

    /* Make a connection request to the server */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)  {
        fprintf(stderr, "Error connecting to socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    /* Get the client request from the command line */
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);     /* read buffer from command line */

    /* Make the client request to the server */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) {
        fprintf(stderr, "Error writing to socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    bzero(buffer,256);

    /* Wait for response from the server */
    n = read(sockfd,buffer,255);
    if (n < 0)  {
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return -1;
    }

    /* Display server response */
    printf("%s\n",buffer);

    close(sockfd);

    return 0;
}

                     