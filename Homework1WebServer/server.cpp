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
void send_404_error_to_client(WebServer *server, string version);
string getHeader(int content_length, string version, string status);
void execute_get_command(WebServer *server, string directory_path, string version, string status);

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

string getHeader(int content_length, string version)
{
  return (getHeader(content_length, version, string("200 OK")));
}

string getHeader(int content_length, string version, string status)
{
  string response = "HTTP/" + version + " " + status +"\n";
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

void send_500_error_to_client(WebServer *server, string version)
{
  cout << "Server sending error to client 500" << endl;
  string response = "HTTP/" + version + " 500 Internal Server Error\n";
  server->WriteMessage(response);
}

void execute_get_command(WebServer *server, string directory_path, string version)
{
  return (execute_get_command(server, directory_path, version, "200 OK"));
}

void execute_get_command(WebServer *server, string directory_path, string version, string status)
{
    printf("Server Sending File %s", directory_path.c_str());
    string line;
    ifstream myfile (directory_path.c_str());
    if (myfile.is_open())
    {
        std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
        int content_length = file_contents.length();
        string header = getHeader(content_length, version, status);
        server->WriteMessage(header + file_contents + "\n\0");
        myfile.close();
    }
    else
    {
      send_404_error_to_client(server, version);
    }
}

void send_404_error_to_client(WebServer *server, string version)
{
  execute_get_command(server, string("html_root/file_not_found.html"), string("404 Not Found"));
}

void remove_if_exists(string directory_path)
{
    ifstream ifile(directory_path.c_str());
    if (ifile)
    {
        printf("Removing file: %s\n",directory_path.c_str());
        remove(directory_path.c_str());
    }
}

void execute_put_command(WebServer *server, string directory_path, string message_body, string version)
{
    remove_if_exists(directory_path);
    std::ofstream out(directory_path.c_str());
    out << message_body;
    out.close();
    execute_get_command(server, string("html_root/file_created.html"), version);
}

void execute_delete_command(WebServer *server, string directory_path, string version)
{
    remove_if_exists(directory_path);
    execute_get_command(server, string("html_root/file_deleted.html"), version);
}

void execute_head_command(WebServer *server, string directory_path, string version)
{
    ifstream myfile (directory_path.c_str());
    std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
    int content_length = file_contents.length();
    string response = getHeader(content_length, version);
    server->WriteMessage(response);
    myfile.close();
}

void routeMessage(string message, WebServer *server)
{
    printf("\nSERVER RECIEVED REQUEST: %s\n", message.c_str());
    vector <string> fields;

    int first_return_index = message.find_first_of('\n');
    string request_only = message.substr(0, first_return_index);
    boost::split( fields, request_only, boost::is_any_of( " " ));

    int header_end_index = message.find("\n\n",0);
    string message_header = message.substr(0, header_end_index);
    string message_body = "";
    if(header_end_index <= message.length() + 2)
    {
      message_body = message.substr(header_end_index + 2, message.length());
    }

    int size = fields.size();
    if(size != 3)
    {
      cout << "p1" << endl;
      send_500_error_to_client(server, "1.1");
      return;
    }

    string command = fields[0];
    string directory = fields[1];
    string protocal_version = fields[2];

    vector <string> fields2;

    boost::split (fields2, protocal_version, boost::is_any_of("//"));

    int size2 = fields2.size();
    if(size2 != 2)
    {
      cout << "p2" << endl;
      send_500_error_to_client(server, "1.1");
      return;
    }

    string protocal = fields2[0];
    string version = fields2[1].substr(0,3);

    printf("command: \e[92m'%s'\e[0m, directory: \e[92m'%s'\e[0m, protocal: \e[92m'%s'\e[0m, version: \e[92m'%s'\e[0m \n ", command.c_str(), directory.c_str(), protocal.c_str(), version.c_str());

    if(directory == "/")
    {
      directory = "/index.html";
    }



    string directory_path = "html_root" + directory;

    if(command == "GET")
    {
        execute_get_command(server, directory_path, version);
    }
    if(command == "HEAD")
    {
        execute_head_command(server, directory_path, version);
    }
    if(command == "PUT")
    {
        execute_put_command(server, directory_path, message_body, version);
    }
    if(command == "DELETE")
    {
        execute_delete_command(server, directory_path, version);
    }
    if(version == "1.0")
    {
      server->CloseConnection();
    }
}
