#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"
#include <pthread.h>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;

void Call_Client(string ipAddress, string portNumber, string version);
void Call_Telnet(string ipAddress, string portNumber, string version);

struct connectionParams{
    string IPAddress;
    string PortNumber;
};

double diffclock( clock_t clock1, clock_t clock2 ) {

       double diffticks = clock1 - clock2;
       double diffms    = diffticks / ( CLOCKS_PER_SEC / 1000 );

       return diffms;
   }

int main(int argc, char *argv[]) {

    if (argc < 3) {
       fprintf(stderr,"usage %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    pthread_t server_thread, client_thread;
    string ipAddress = argv[1];
    string portNumber = argv[2];

    clock_t start = clock();
    clock_t t = clock();


    Call_Client(ipAddress, portNumber, "1.0");
    //Call_Telnet(ipAddress, portNumber, "1.0");

    t = clock() - t;
    clock_t end = clock();

    printf("diffclock: %f", diffclock( start, end ));
    printf("totaltime: %f", t);

    //Call_Client(ipAddress, portNumber, "1.0");

    return 0;
}

void Call_Telnet(string ipAddress, string portNumber, string version)
{
  string command = "telnet " + ipAddress + " " + portNumber;

  system(command.c_str());
  //system("GET / HTTP/1.0");
}

void Call_Client(string ipAddress, string portNumber, string version)
{
  printf("CLIENT: Client Connecting to Web Server at IP:%s Port:%s", ipAddress.c_str(), portNumber.c_str());
  WebClient* aclient = new WebClient();
  printf("\nCLENT: Connecting To Server\n");
  aclient->Connect(ipAddress, portNumber, version);
  printf("\nCLENT: Sending Get Request\n");

  for(int i = 0; i < 100; i++)
  {
    int duration = 0;

    printf ("\nNum of Runs: %d\n", i);
    aclient->SendRequest("HEAD / HTTP/" + version);
    usleep(duration);

    aclient->SendRequest("HEAD /large_file.html HTTP/" + version);
    usleep(duration);

    aclient->SendRequest("HEAD /missing_file.html HTTP/" + version);
    usleep(duration);

    aclient->SendRequest("GET / HTTP/" + version);
    usleep(duration);
    aclient->SendRequest("GET /large_file.html HTTP/" + version);
    usleep(duration);
    aclient->SendRequest("GET /missing_file.html HTTP/" + version);
    usleep(duration);

    string put_request = "PUT /stuff.html HTTP/" + version + "\n";
    put_request += "User-Agent: SomeClient 1.0\n";
    put_request += "Host: "+ ipAddress+":" + portNumber + "\n";
    put_request += "Accept: */*\n";
    put_request += "Content-Type: text/html; charset=UTF-8\n";
    put_request += "Content-Length:14\n\n";
    put_request += "Put Command Contents For File.";
    aclient->SendRequest(put_request);
    usleep(duration);

    aclient->SendRequest("GET /stuff.html HTTP/" + version);
    usleep(duration);
    aclient->SendRequest("DELETE /stuff.html HTTP/" + version);
    usleep(duration);
    aclient->SendRequest("GET /stuff.html HTTP/" + version);

    usleep(duration);
  }
  printf("\nCLENT: Request Finished. Destroying Client\n");
  delete aclient;
  //aclient->~WebClient();
}
