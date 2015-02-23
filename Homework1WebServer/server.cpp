#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "WebServer.h"
#include "WebClient.h"
#include <pthread.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <streambuf>
#include <vector>
using namespace std;
using namespace boost;

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
  printf("2");
    printf("\nSERVER RECIEVED REQUEST: %s\n", message.c_str());
    printf("newone");
    vector <string> fields;

    split( fields, message, is_any_of( " " ));
    printf("split");

    //TODO: Add error checking

    string command = fields[0];
    string directory = fields[1];
    string protocal_version = fields[2];

    printf("undervar1");
    vector <string> fields2;

    split (fields2, protocal_version, is_any_of("//"));
    printf("var2");

    string protocal = fields2[0];
    string version = fields2[1];

    printf("command: %s, directory: %s, protocal: %s, version: %s", command.c_str(), directory.c_str(), protocal.c_str(), version.c_str());

    if(directory == "//")
    {
      directory = "/index.html";
    }

    if(command == "GET")
    {
      string directory_path = "html_root" + directory;
      printf ("between1");

      string line;
      printf ("between2");
      ifstream myfile (directory_path.c_str());
      printf ("between3");
      if (myfile.is_open())
      {
        printf ("between4");
        while ( getline (myfile,line) )
        {
          printf ("while");
          printf("line: %s", line.c_str());
        }
        printf ("between5");
        myfile.close();
      }
      printf ("between6");
      /*ifstream t(directory_path.c_str(), std::ios::in|std::ios::ate);
      string str;

      if(!t )
      {
        cout << "file not loaded" << endl;
      }

      t.seekg(0, ios::end);
      cout << "a" << endl;

      streampos filesize = t.tellg();
      cout << "c" << endl;

      while (!t.eof())
      {
        str = t.get();
        cout << str;
      }*/


      //str.reserve(t.tellg());
      //cout << "b" << endl;
      /*t.seekg(0, ios::beg);

      cout << "h" << endl;
      str.assign((istreambuf_iterator<char>(t)),
                  istreambuf_iterator<char>());
      cout << "p" << endl;

      cout << "str: " << str << endl;//*/

    }
}
