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

void WebClient::SendGetRequestAndAwaitResponse(){
    int rc = 0; // Actual number of bytes read by function read()
    char buffer[256];

    strcpy(buffer, "GET / HTTP/1.0");
    int failedWhenNegative = write(socketHandle, buffer, 255);

    if (failedWhenNegative < 0)  {
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    bzero(buffer,256);

    failedWhenNegative = read(socketHandle,buffer,255);
    if (failedWhenNegative < 0)  {
      cout << "client failed" << endl;
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    /* Display server response */
    printf("%s\n",string(buffer).c_str());

}

WebClient::~WebClient() {
    //close(socketHandle);
}
