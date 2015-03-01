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


    printf("CLIENT: Client Connecting to Web Server at IP:%s Port:%s", ipAddress.c_str(), portNumber.c_str());
    WebClient* aclient = new WebClient();
    printf("\nCLENT: Connecting To Server\n");
    aclient->Connect(ipAddress, portNumber);
    printf("\nCLENT: Sending Get Request\n");

    aclient->SendRequest("HEAD / HTTP/1.0");
    //aclient->SendRequest("HEAD /large_file.html HTTP/1.0");
    //aclient->SendRequest("HEAD /missing_file.html HTTP/1.0");

    aclient->SendRequest("GET / HTTP/1.0");
    //aclient->SendRequest("GET /large_file.html HTTP/1.0");
    //aclient->SendRequest("GET /missing_file.html HTTP/1.0");

    string put_request = "PUT /stuff.html HTTP/1.0\n";
    put_request += "User-Agent: SomeClient 1.0\n";
    put_request += "Host: "+ ipAddress+":" + portNumber + "\n";
    put_request += "Accept: */*\n";
    put_request += "Content-Type: text/html; charset=UTF-8\n";
    put_request += "Content-Length:14\n\n";
    put_request += "Put Command Contents For File.";
    aclient->SendRequest(put_request);

    aclient->SendRequest("GET /stuff.html HTTP/1.0");

    aclient->SendRequest("DELETE /stuff.html HTTP/1.0");

    aclient->SendRequest("GET /stuff.html HTTP/1.0");

    printf("\nCLENT: Request Finished. Destroying Client\n");
    aclient->~WebClient();
    return 0;
}
