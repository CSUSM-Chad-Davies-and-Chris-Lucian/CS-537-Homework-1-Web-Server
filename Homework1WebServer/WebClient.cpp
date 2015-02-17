#include "WebClient.h"

WebClient::WebClient() {
}

WebClient::WebClient(const WebClient& orig) {
}

void WebClient::Connect(string ipAddress, string port){
    
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int portInt = stoi(port);
    socketInfo.sin_port = htons(portInt);

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socketHandle < 0)
    {
        //Socket creation failed
    }
    
    int connectionSuccess = connect(socketHandle, (struct sockaddr *) &socketInfo, sizeof (sockaddr_in));
    if(connectionSuccess < 0)
    {
        //Connection failed
    }
    
}

void WebClient::SendGetRequestAndAwaitResponse(){
    int rc = 0; // Actual number of bytes read by function read()
    char buf[512];

    strcpy(buf, "Message to send");
    send(socketHandle, buf, strlen(buf) + 1, 0);
}

WebClient::~WebClient() {
    close(socketHandle);
}

