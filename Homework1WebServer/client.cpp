//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

using namespace std;

// Function declarations
void Call_Client(string ipAddress, string portNumber, string version);
timespec diff(timespec start, timespec end);

//This program is a client meant to performance test the web server
//The arguments are the ipAddress of the server and the servers port
int main(int argc, char *argv[]) {

    //Argument Validation
    if (argc < 3) {
       fprintf(stderr,"usage %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    //Assign the arguments to well-named variables
    string ipAddress = argv[1];
    string portNumber = argv[2];

    //Get the start time of the HTTP 1.0 stress test run
    timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    //Call the client functions for the HTTP 1.0 protocol
    Call_Client(ipAddress, portNumber, "1.0");

    //Get the end time of the HTTP 1.0 stress test run
    timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    //Get the start time of the HTTP 1.1 stress test run
    timespec start2;
    clock_gettime(CLOCK_REALTIME, &start2);

    //Call the client functions for the HTTP 1.1 protocol
    Call_Client(ipAddress, portNumber, "1.1");

    //Get the end time of the HTTP 1.1 stress test run
    timespec end2;
    clock_gettime(CLOCK_REALTIME, &end2);

    //Print the results of the stress test in (Seconds:Nanoseconds)
    cout<< "Protocol 1.0: " <<diff(start,end).tv_sec<<":"<<diff(start,end).tv_nsec<<endl;
    cout<< "Protocol 1.1: " <<diff(start2,end2).tv_sec<<":"<<diff(start2,end2).tv_nsec<<endl;

    return 0;
}

//Get the difference between 2 times
timespec diff(timespec start, timespec end)
{
	timespec temp;
  //If end Nanoseconds is greater than start Nanoseconds
	if ((end.tv_nsec-start.tv_nsec)<0) {
    //Adjust seconds and Nanoseconds to accommodate the offset
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
    //otherwise do a simple subtraction
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

//This function creates a web client and attempts to run 100 calls against the server
void Call_Client(string ipAddress, string portNumber, string version)
{
  //Create the client
  printf("CLIENT: Client Connecting to Web Server at IP:%s Port:%s", ipAddress.c_str(), portNumber.c_str());
  WebClient* aclient = new WebClient();

  //Connect to the specified server
  printf("\nCLENT: Connecting To Server\n");
  aclient->Connect(ipAddress, portNumber, version);

  //Loop 10 times over the 10 requests
  printf("\nCLENT: Starting stress test\n");
  for(int i = 0; i < 10; i++)
  {
    //duration set to 0 however when set above 0 this can be used for debugging
    int duration = 0;

    printf ("\nNum of Runs: %d\n", i);

    //Get the header for the index page
    aclient->SendRequest("HEAD / HTTP/" + version);
    usleep(duration);

    //Get the header for the large file
    aclient->SendRequest("HEAD /large_file.html HTTP/" + version);
    usleep(duration);

    //Attempt to get the header for a missing file
    aclient->SendRequest("HEAD /missing_file.html HTTP/" + version);
    usleep(duration);

    //Get the index file
    aclient->SendRequest("GET / HTTP/" + version);
    usleep(duration);

    //Get a large file of lorem ipsum
    aclient->SendRequest("GET /large_file.html HTTP/" + version);
    usleep(duration);

    //Attempt a get request
    aclient->SendRequest("GET /missing_file.html HTTP/" + version);
    usleep(duration);

    //Build a put statement with file contents and send request
    string put_request = "PUT /stuff.html HTTP/" + version + "\n";
    put_request += "User-Agent: SomeClient 1.0\n";
    put_request += "Host: "+ ipAddress+":" + portNumber + "\n";
    put_request += "Accept: */*\n";
    put_request += "Content-Type: text/html; charset=UTF-8\n";
    put_request += "Content-Length:14\n\n";
    put_request += "Put Command Contents For File.";
    aclient->SendRequest(put_request);
    usleep(duration);

    //Get the file that was created by the put command
    aclient->SendRequest("GET /stuff.html HTTP/" + version);
    usleep(duration);
    //Delete the file that was created by the put command
    aclient->SendRequest("DELETE /stuff.html HTTP/" + version);
    usleep(duration);

    //Get the file that was deleted to verify it was deleted.
    aclient->SendRequest("GET /stuff.html HTTP/" + version);
    usleep(duration);
  }

  //Delete the pointer to the client
  printf("\nCLENT: Request Finished. Destroying Client\n");
  delete aclient;
}
