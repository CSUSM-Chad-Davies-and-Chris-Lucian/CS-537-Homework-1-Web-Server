//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

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

//Function declaratins
void* thread_start_server(void *context);
void routeMessage(string message, WebServer *server, int socketConnection);
void send_404_error_to_client(WebServer *server, string version, int socketConnection);
string getHeader(int content_length, string version, string status, int socketConnection);
void execute_get_command(WebServer *server, string directory_path, string version, string status, int socketConnection);

//Structure to old connection parameters
struct connectionParams{
    string IPAddress;
    string PortNumber;
};

//Runs the server accepting the port that the server should be hosted on
int main(int argc, char *argv[]) {

    // Validate the correct number of arguments have been passed to the server
    if (argc < 2) {
        fprintf(stderr,"Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //Create a thread for the server to run on
    pthread_t server_thread;
    string portNumber = argv[1];

    //sets up the connection params to be sent to the thread
    struct connectionParams params;
    params.PortNumber = portNumber;

    //Creates the thread
    printf("\nSERVER MAIN: Starting server thread\n");
    pthread_create(&server_thread, 0, thread_start_server,&params);

    //Join to prevent application from exiting
    printf("\nSERVER MAIN: Joining server thread\n");
    pthread_join(server_thread,0);

    //End the program
    printf("\nSERVER MAIN: Ending Program\n");
    return 0;
}

//A function to be used by the thread create in order to host the server
void *thread_start_server(void *context) {
    //retreives the connecton params from the variable passed by the thread create
    struct connectionParams *params = (struct connectionParams*) context;

    //start the server and host it on the specified port
    printf("SERVER: Constructing Web Server at IP:%s Port:%s", params->IPAddress.c_str(), params->PortNumber.c_str());
    WebServer* aserver = new WebServer(params->PortNumber);

    //start listening and pass in the function pointer to the route message function
    printf("\nSERVER: Starting Listener\n");
    aserver->StartListening(&routeMessage);
    printf("\nSERVER: Finished Listening\n");

    //if the server exits the listening loop, dispose of it properly
    delete aserver;
    printf("\nSERVER: Web Server Destroyed.\n");
}

//Overload of the get header method that assumes 200 OK is being passed back to the client
string getHeader(int content_length, string version, int socketConnection)
{
  return (getHeader(content_length, version, string("200 OK"), socketConnection));
}

//Primary get header function call is used to construct and return the header as a string
//Since the header is sent back with each request this function is called in each response.
//Version parameter is used to specify the response HTTP version
//Content Length parameter is used to fill the content length header field
//Status parameter is used to set the HTTP status like 200 OK or 404 Not Found
string getHeader(int content_length, string version, string status, int socketConnection)
{
  //Construct the response version and status
  string response = "HTTP/" + version + " " + status +"\n";
  //add server information
  response += "Server: simpleServer/1.0\n";

  //construct the date for the response
  char buf[1000];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);
  response += "Date: " + string(buf) + "\n";

  //set the content type of the response
  response += "Content-type: text/html; charset=UTF-8\n";

  //construct the content length portion of the resrponse.
  string length_string = boost::lexical_cast<std::string>(content_length);
  response += "Content-Length: " + length_string + "\n\n";
  return response;
}

//Sends a error 500 back to the client in any bad situation
//If the http protocol is violated for example
void send_500_error_to_client(WebServer *server, string version, int socketConnection)
{
  //Log error to output
  cout << "Server sending error to client 500" << endl;
  //construct the response
  string response = "HTTP/" + version + " 500 Internal Server Error\n";
  //Send the response back to the client
  server->WriteMessage(response, socketConnection);
}

//Overload of the get command assuming the return status is 200 OK
void execute_get_command(WebServer *server, string directory_path, string version, int socketConnection)
{
  return (execute_get_command(server, directory_path, version, "200 OK", socketConnection));
}

//Execturtes the get command
//Server parameter is used to send the response to the client
//Directory Path is used to load files form the server to send to the client
//Version parameter is used to set the HTTP version in the header response
//Status parameter is used to specify the HTTP response status
//socketConnection parameter is used to sell the server which connection to send the response to
void execute_get_command(WebServer *server, string directory_path, string version, string status, int socketConnection)
{
    //log the file being sent to the client on the server output
    printf("Server Sending File %s", directory_path.c_str());
    //atempt to opent he file
    string line;
    ifstream myfile (directory_path.c_str());
    //If file was opened then it was found
    if (myfile.is_open())
    {
        //Load the file
        std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
        //Get the content length to populate in the header
        int content_length = file_contents.length();
        //Get the header
        string header = getHeader(content_length, version, status, socketConnection);
        //prepend the header and send it to the client
        server->WriteMessage(header + file_contents + "\n\0", socketConnection);
        //close the file
        myfile.close();
    }
    else
    {
      //If the file was not loaded and opened then send a 404 back to the client
      send_404_error_to_client(server, version, socketConnection);
    }
}

//Send 404 error back to the client by executing get command with a 404 status code for the file not found file
void send_404_error_to_client(WebServer *server, string version, int socketConnection)
{
  execute_get_command(server, string("html_root/file_not_found.html"), string("404 Not Found"), socketConnection);
}

//Deletes a file if it exists
//Used for both the delete command and the put command
//Directory path parameter is used to identify the file to be deleted
void remove_if_exists(string directory_path)
{
    //open the file
    ifstream ifile(directory_path.c_str());
    //if the file exists
    if (ifile)
    {
        //delete the file
        printf("Removing file: %s\n",directory_path.c_str());
        remove(directory_path.c_str());
    }
}

//execture the put command
void execute_put_command(WebServer *server, string directory_path, string message_body, string version, int socketConnection)
{
    //delete the file in anticipation of the put
    remove_if_exists(directory_path);
    //write the file to the directory
    std::ofstream out(directory_path.c_str());
    out << message_body;
    out.close();
    //return the confirmation to the client with the file created html in the return
    execute_get_command(server, string("html_root/file_created.html"), version, socketConnection);
}

//execture the delete command
void execute_delete_command(WebServer *server, string directory_path, string version, int socketConnection)
{
    //Delete the file
    remove_if_exists(directory_path);
    //return a delete success message back to the client
    execute_get_command(server, string("html_root/file_deleted.html"), version, socketConnection);
}

//execture the head command
void execute_head_command(WebServer *server, string directory_path, string version, int socketConnection)
{
    //open the file
    ifstream myfile (directory_path.c_str());
    //read the file to a string
    std::string file_contents((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());
    //get the content length
    int content_length = file_contents.length();
    //construct the header
    string response = getHeader(content_length, version, socketConnection);
    //write the response back to the client
    server->WriteMessage(response, socketConnection);
    //close the file
    myfile.close();
}

//Routes messages that come from the web server object recieved from connected clients
void routeMessage(string message, WebServer *server, int socketConnection)
{
    //reuqest has been recieved, log the unparsed message to the console
    printf("\nSERVER RECIEVED REQUEST: %s\n", message.c_str());
    vector <string> fields;

    //split on new lines and get the first to isolate the request
    //This is done specifically for the put command
    int first_return_index = message.find_first_of('\n');
    string request_only = message.substr(0, first_return_index);
    //split the isolated request on spaces to seperate the request info
    boost::split( fields, request_only, boost::is_any_of( " " ));

    //isolate the message body from the header
    int header_end_index = message.find("\n\n",0);
    string message_header = message.substr(0, header_end_index);
    string message_body = "";
    if(header_end_index <= message.length() + 2)
    {
      message_body = message.substr(header_end_index + 2, message.length());
    }

    //verify the proper formatting of the HTTP request
    int size = fields.size();
    if(size != 3)
    {
      send_500_error_to_client(server, "1.1", socketConnection);
      return;
    }

    //assign variables to the command compoentns
    string command = fields[0];
    string directory = fields[1];
    string protocal_version = fields[2];

    //split the protocol and version
    vector <string> fields2;
    boost::split (fields2, protocal_version, boost::is_any_of("//"));

    //verify the protocol and version were properly formatted
    int size2 = fields2.size();
    if(size2 != 2)
    {
      send_500_error_to_client(server, "1.1", socketConnection);
      return;
    }

    //assign variables to protocol and version
    string protocal = fields2[0];
    string version = fields2[1].substr(0,3);

    // Log commands composition back to console
    printf("command: \e[92m'%s'\e[0m, directory: \e[92m'%s'\e[0m, protocal: \e[92m'%s'\e[0m, version: \e[92m'%s'\e[0m \n ", command.c_str(), directory.c_str(), protocal.c_str(), version.c_str());

    // changes the / to index file
    if(directory == "/")
    {
      directory = "/index.html";
    }

    // constructs the directory path
    string directory_path = "html_root" + directory;

    // this if block routes the command to the appropriate function
    if(command == "GET")
    {
        execute_get_command(server, directory_path, version, socketConnection);
    }
    else if(command == "HEAD")
    {
        execute_head_command(server, directory_path, version, socketConnection);
    }
    else if(command == "PUT")
    {
        execute_put_command(server, directory_path, message_body, version, socketConnection);
    }
    else if(command == "DELETE")
    {
        execute_delete_command(server, directory_path, version, socketConnection);
    }
    else // if no command matches return an error to the client
    {
        send_500_error_to_client(server, version, socketConnection);
    }

    // end the connection if version is 1.0
    if(version == "1.0")
    {
      server->CloseConnection(socketConnection);
    }
}
