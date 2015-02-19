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
