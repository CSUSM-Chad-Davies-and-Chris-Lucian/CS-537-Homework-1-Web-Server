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
    char buffer[256];

    strcpy(buffer, request.c_str());
    int failedWhenNegative = write(socketHandle, buffer, 255);

    if (failedWhenNegative < 0)  {
        printf("Client Read Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }


    bzero(buffer,256);
    failedWhenNegative = read(socketHandle,buffer,255);
    if (failedWhenNegative < 0)  {
        printf("Client Write Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }
    string result = string(buffer);
    printf("%s\n",result.c_str());
}

WebClient::~WebClient() {
    //close(socketHandle);
}
