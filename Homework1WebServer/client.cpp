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

void* thread_connect_client(void *ptr);

struct connectionParams{
    string IPAddress;
    string PortNumber;
};

int main(int argc, char *argv[]) {

    if (argc < 3) {
       fprintf(stderr,"usage %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    pthread_t server_thread, client_thread;
    string ipAddress = argv[1];
    string portNumber = argv[2];

    struct connectionParams params;
    params.IPAddress = ipAddress;
    params.PortNumber = portNumber;

    printf("\nCLIENT MAIN: Starting client thread\n");
    pthread_create(&client_thread, 0, thread_connect_client,&params);


    printf("\nCLIENT MAIN: Joining client thread\n");
    pthread_join(client_thread, 0);

    printf("\nCLIENT MAIN: Ending Program\n");
    return 0;
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
