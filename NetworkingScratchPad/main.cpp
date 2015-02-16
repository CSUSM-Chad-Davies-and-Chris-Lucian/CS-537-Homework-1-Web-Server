#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"
#include <pthread.h>


void* thread_start_server(void *ptr);
void* thread_connect_client(void *ptr);

int main(int argc, char *argv[]) {
    
    pthread_t server_thread, client_thread;
    
    
    
    printf("\nMAIN: Please enter the host port number.\n");
    //scanf("%s", &portNumber);
    
    printf("\nMAIN: Please enter the host IP address.\n");
    //scanf("%s",&ipAddress);
    
    printf("\nMAIN: Starting server thread\n");
    pthread_create(&server_thread, 0, thread_start_server,0);
    printf("\nMAIN: Starting client thread\n");
    pthread_create(&client_thread, 0, thread_connect_client,0);


    printf("\nMAIN: Joining client thread\n");
    pthread_join(client_thread, 0);
    printf("\nMAIN: Joining server thread\n");
    pthread_join(server_thread,0);
    
    printf("\nMAIN: Ending Program\n");
    return 0;
}

void *thread_start_server(void *ptr) {
    char* portNumber = "80";
    char* ipAddress = "127.0.0.1";
    
    printf("\nSERVER: Constructing Web Server\n");
    WebServer* aserver = new WebServer(portNumber);
    printf("\nSERVER: Starting Listener\n");
    aserver->StartListening();
    printf("\nSERVER: Finished Listening\n");
    aserver->~WebServer();
    printf("\nSERVER: Web Server Destroyed.\n");
}

void *thread_connect_client(void *ptr) {
    char* portNumber = "80";
    char* ipAddress = "127.0.0.1";
    
    printf("\nCLENT: Constructing Client\n");
    WebClient* aclient = new WebClient();
    printf("\nCLENT: Connecting To Server\n");
    aclient->Connect(ipAddress, portNumber);
    printf("\nCLENT: Sending Get Request\n");
    aclient->SendGetRequestAndAwaitResponse();
    printf("\nCLENT: Request Finished. Destroying Client\n");
    aclient->~WebClient();
}