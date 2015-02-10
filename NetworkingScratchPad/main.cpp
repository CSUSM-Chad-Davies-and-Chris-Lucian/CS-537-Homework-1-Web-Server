#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "WebServer.h"
#include "WebClient.h"

int main(int argc, char *argv[]) {
    
    int portNumer = 122000;
    
    WebServer* aserver = new WebServer(portNumer);
    
    aserver->StartListening();
    
    WebClient* aclient = new WebClient();
    
    aclient->Connect("127.0.0.1", portNumer);
    
    
    return 0;
}