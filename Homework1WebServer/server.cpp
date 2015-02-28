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
#include <boost/lexical_cast.hpp>
#include <time.h>
using namespace std;
//using namespace boost;

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

string getHeader(int content_length)
{
  string response = "HTTP/1.0 200 OK\n";
  response += "Server: simpleServer/1.0\n";
  char buf[1000];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);
  response += "Date: " + string(buf) + "\n";
  response += "Content-type: text/html; charset=UTF-8\n";
  string length_string = boost::lexical_cast<std::string>(content_length);
  response += "Content-Length: " + length_string + "\n\n";
  return response;
}

void execture_get_command(WebServer *server, string directory_path)
{
    printf("Server Sending File %s", directory_path.c_str());
    string line;
    ifstream myfile (directory_path.c_str());
    if (myfile.is_open())
    {
        std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
        int content_length = file_contents.length();
        string header = getHeader(content_length);
        server->WriteMessage(header + file_contents);
        myfile.close();
    }
}

void execture_head_command(WebServer *server, string directory_path)
{
    ifstream myfile (directory_path.c_str());
    std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
    int content_length = file_contents.length();
    string response = getHeader(content_length);
    server->WriteMessage(response);
    myfile.close();
}

void routeMessage(string message, WebServer *server)
{
    printf("\nSERVER RECIEVED REQUEST: %s\n", message.c_str());

    vector <string> fields;

    boost::split( fields, message, boost::is_any_of( " " ));

    //TODO: Add error checking

    string command = fields[0];
    string directory = fields[1];
    string protocal_version = fields[2];

    vector <string> fields2;

    boost::split (fields2, protocal_version, boost::is_any_of("//"));

    string protocal = fields2[0];
    string version = fields2[1];


    printf("command: '%s', directory: '%s', protocal: '%s', version: '%s'\n", command.c_str(), directory.c_str(), protocal.c_str(), version.c_str());

    if(directory == "/")
    {
      directory = "/index.html";
    }

    string directory_path = "html_root" + directory;

    if(command == "GET")
    {
        execture_get_command(server, directory_path);
    }
    if(command == "HEAD")
    {
        execture_head_command(server, directory_path);
    }
    if(command == "PUT")
    {
    }
    if(command == "DELETE")
    {
    }
}
