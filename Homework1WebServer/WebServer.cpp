#include "WebServer.h"

WebServer::WebServer(string portNumer) {

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    if(socketHandle < 0)
    {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
              errno, strerror(errno));
        return;
    }

    bzero((char *) &socketInfo, sizeof(socketInfo));
    int portInt = atoi(portNumer.c_str());
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    socketInfo.sin_port = htons(portInt);

    int binding = bind(socketHandle,(struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in));
    if(binding < 0)
    {
        fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    listen(socketHandle,1);
}

void WebServer::StartListening(){
    if(socketHandle >= 0)
    {
        int socketConnection = accept(socketHandle, 0, 0);
        if(socketConnection < 0)
        {
            fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                    errno, strerror(errno));
            return;
        }
        close(socketHandle);

        int rc = 0; // Actual number of bytes read
        char buf[512];
        rc = recv(socketConnection, buf, 512, 0);
        buf[rc] = (char) NULL;
    }
}

WebServer::~WebServer() {
    close(socketHandle);
}
