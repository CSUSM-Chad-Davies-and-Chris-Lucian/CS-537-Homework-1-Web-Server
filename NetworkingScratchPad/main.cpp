#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"

int main(int argc, char *argv[]) {
    
    printf("Initializing Web Server...");
    
    char* portNumber = "12640";
    char* ipAddress = "127.0.0.1";
    
    printf("\nPlease enter the host port number.\n");
    //scanf("%s", &portNumber);
    
    
    
    printf("\nPlease enter the host IP address.\n");
    //scanf("%s",&ipAddress);
    
    WebServer* aserver = new WebServer(portNumber);
    aserver->StartListening();

    
    
    WebClient* aclient = new WebClient();
    aclient->Connect(ipAddress, portNumber);
    aclient->SendGetRequestAndAwaitResponse();
    
    aclient->~WebClient();
    aserver->~WebServer();
    return 0;
}