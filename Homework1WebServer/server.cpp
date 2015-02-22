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

void* thread_start_server(void *context);
void routeMessage(string message, WebServer *server);

struct connectionParams{
    string IPAddress;
    string PortNumber;
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr,"Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_t server_thread, client_thread;
    string portNumber = argv[1];

    struct connectionParams params;
    params.PortNumber = portNumber;

    printf("\nSERVER MAIN: Starting server thread\n");
    pthread_create(&server_thread, 0, thread_start_server,&params);

    printf("\nSERVER MAIN: Joining server thread\n");
    pthread_join(server_thread,0);

    printf("\nSERVER MAIN: Ending Program\n");
    return 0;
}

void *thread_start_server(void *context) {
    struct connectionParams *params = (struct connectionParams*) context;
    printf("SERVER: Constructing Web Server at IP:%s Port:%s", params->IPAddress.c_str(), params->PortNumber.c_str());
    WebServer* aserver = new WebServer(params->PortNumber);
    printf("\nSERVER: Starting Listener\n");
    aserver->StartListening(&routeMessage);
    printf("\nSERVER: Finished Listening\n");
    aserver->~WebServer();
    printf("\nSERVER: Web Server Destroyed.\n");
}

void routeMessage(string message, WebServer *server)
{
    printf("\nSERVER RECIEVED REQUEST: %s\n", message.c_str());
}
