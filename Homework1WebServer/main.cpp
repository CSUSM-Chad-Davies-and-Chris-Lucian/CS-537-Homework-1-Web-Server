#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"
#include <pthread.h>
#include <string>
#include <iostream>
using namespace std;

void* thread_start_server(void *ptr);
void* thread_connect_client(void *ptr);

struct connectionParams{
    string IPAddress;
    string PortNumber;
};

int main(int argc, char *argv[]) {
    
    pthread_t server_thread, client_thread;
    
    string ipAddress;
    string portNumber;

    printf("\nMAIN: Please enter the host IP address.\n");
    cin >> ipAddress;
    
    printf("\nMAIN: Please enter the host port number.\n");
    cin >> portNumber;
    
    struct connectionParams params;
    params.IPAddress = ipAddress; 
    params.PortNumber = portNumber;
    
    printf("\nMAIN: Starting server thread\n");
    pthread_create(&server_thread, 0, thread_start_server,&params);
    printf("\nMAIN: Starting client thread\n");
    pthread_create(&client_thread, 0, thread_connect_client,&params);


    printf("\nMAIN: Joining client thread\n");
    pthread_join(client_thread, 0);
    printf("\nMAIN: Joining server thread\n");
    pthread_join(server_thread,0);
    
    printf("\nMAIN: Ending Program\n");
    return 0;
}

void *thread_start_server(void *context) {
    struct connectionParams *params = (struct connectionParams*) context;
    printf("SERVER: Constructing Web Server at IP:%s Port:%s", params->IPAddress.c_str(), params->PortNumber.c_str());
    WebServer* aserver = new WebServer(params->PortNumber);
    printf("\nSERVER: Starting Listener\n");
    aserver->StartListening();
    printf("\nSERVER: Finished Listening\n");
    aserver->~WebServer();
    printf("\nSERVER: Web Server Destroyed.\n");
}

void *thread_connect_client(void *context) {
    struct connectionParams *params = (struct connectionParams*) context;
    string portNumber = params->PortNumber;
    string ipAddress = params->IPAddress;
    
    printf("CLIENT: Client Connecting to Web Server at IP:%s Port:%s", params->IPAddress.c_str(), params->PortNumber.c_str());
    WebClient* aclient = new WebClient();
    printf("\nCLENT: Connecting To Server\n");
    aclient->Connect(ipAddress, portNumber);
    printf("\nCLENT: Sending Get Request\n");
    aclient->SendGetRequestAndAwaitResponse();
    printf("\nCLENT: Request Finished. Destroying Client\n");
    aclient->~WebClient();
}