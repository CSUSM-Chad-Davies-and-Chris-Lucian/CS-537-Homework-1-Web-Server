#include "WebServer.h"

WebServer::WebServer(string portNumer) {

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    if(socketHandle < 0)
    {
        // socket creation failed
    }

    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    int portInt = atoi(portNumer.c_str());
    socketInfo.sin_port = htons(portInt);
    
}

void WebServer::StartListening(){
    if(socketHandle >= 0)
    {
        int binding = bind(socketHandle,(struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in));
        if(binding < 0)
        {
            //binding failed
        }

        listen(socketHandle,1);

        int socketConnection = accept(socketHandle, 0, 0);
        if(socketConnection < 0)
        {
            //connection failed
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
