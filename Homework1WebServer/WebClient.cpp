#include "WebClient.h"
WebClient::WebClient() {
}

void WebClient::Connect(string ipAddress, string port){

    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int portInt = atoi(port.c_str());
    socketInfo.sin_port = htons(portInt);

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    if(socketHandle < 0)
    {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }


    int connectionSuccess = connect(socketHandle, (struct sockaddr *) &socketInfo, sizeof (sockaddr_in));
    if(connectionSuccess < 0)
    {
        fprintf(stderr, "Error connecting to socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

}

void WebClient::SendRequest(string request){
    int rc = 0; // Actual number of bytes read by function read()
    char buffer[4000];
    printf("Client Sending Request %s\n", request.c_str());
    strcpy(buffer, request.c_str());
    int failedWhenNegative = write(socketHandle, buffer, 4000);

    if (failedWhenNegative < 0)  {
        printf("Client Read Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }
    string result;
    int bytes_read = 0;
    int total_to_read = 4000;
    bzero(buffer,4000);
    bytes_read = read(socketHandle,buffer,4000);
    if (failedWhenNegative < 0)  {
        printf("Client Write Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }
    total_to_read -= bytes_read;
    result = result + string(buffer);
    printf("%s\n",result.c_str());

}

WebClient::~WebClient() {
    //close(socketHandle);
}
