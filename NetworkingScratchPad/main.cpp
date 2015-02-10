#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

int main(int argc, char *argv[]) {
    
    int portNumer = 122000;
    
    AwesomeServer aserver = new AwesomeServer(portNumer);
    
    aserver.ListenForNewConnections();
    
    AwesomeClient aclient = new AwesomeClient();
    
    aclient.Connect("127.0.0.1", portNumer);
    
    
    return 0;
}