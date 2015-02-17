#include "WebServer.h"

//What is c++ member level variable declaration

WebServer::WebServer(char* portNumer) {
    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    if(socketHandle < 0)
    {
        // socket creation failed
    }
    

    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    socketInfo.sin_port = htons(80);
}

WebServer::WebServer(const WebServer& orig) {
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
        
    }
}

WebServer::~WebServer() {
  close(socketHandle);
}

