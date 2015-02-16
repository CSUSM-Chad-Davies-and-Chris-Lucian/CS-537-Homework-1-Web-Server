#include "WebClient.h"

WebClient::WebClient() {
}

WebClient::WebClient(const WebClient& orig) {
}

void WebClient::Connect(char* ipAddress, char* port){
    
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socketInfo.sin_port = htons(80);

    
    
}

void WebClient::SendGetRequestAndAwaitResponse(){
    
}

WebClient::~WebClient() {
}

