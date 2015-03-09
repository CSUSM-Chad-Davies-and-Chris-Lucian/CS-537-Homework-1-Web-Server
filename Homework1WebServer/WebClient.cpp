//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#include "WebClient.h"

// constructor empty no setup done before connection
WebClient::WebClient() {
}

// establish connection to server with given ip address port and http version
void WebClient::Connect(string ipAddress, string port, string http_version){

    version = http_version;

    // setup sockets
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int portInt = atoi(port.c_str());
    socketInfo.sin_port = htons(portInt);

    // establish continous connection for version 1.1
    if(http_version == "1.1")
    {
      ConnectSocket();
    }
}

// connects the socket
void WebClient::ConnectSocket()
{
  // creates the socket
  socketHandle = socket(AF_INET, SOCK_STREAM, 0);

  // closes on error
  if(socketHandle < 0)
  {
      fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
              errno, strerror(errno));
              close(socketHandle);
      return;
  }

  // establish connection
  int connectionSuccess = connect(socketHandle, (struct sockaddr *) &socketInfo, sizeof (sockaddr_in));

  // closes on error
  if(connectionSuccess < 0)
  {
      fprintf(stderr, "Error connecting to socket, errno = %d (%s) \n",
              errno, strerror(errno));
              close(socketHandle);
      return;
  }
}

// sends a string to the server
void WebClient::SendRequest(string request){

  // connect socket on 1.0
  if(version == "1.0")
  {
    ConnectSocket();
  }

    // sets up buffer for communication
    int rc = 0; // Actual number of bytes read by function read()
    int size = 100000;
    char buffer[size];
    bzero(buffer,size);

    // logs the request to the console
    printf("Client Sending Request %s\n", request.c_str());

    // writes the request to the socket
    strcpy(buffer, request.c_str());
    int failedWhenNegative = write(socketHandle, buffer, request.length());

    // closes on error
    if (failedWhenNegative < 0)
    {

        printf("Client Read Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    // resets buffer to read reply
    string result;
    int bytes_read = 0;
    int total_to_read = size;

    bzero(buffer,size);

    // reads reply from socket
    bytes_read = read(socketHandle,buffer,total_to_read);

    // closes on error
    if (bytes_read < 0)
    {
        printf("Client Write Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }


    total_to_read -= bytes_read;
    result = result + string(buffer);
    printf("Client Result: %s", result.c_str());

    // if version is 1.0 then close conneciton
    if(version == "1.0")
    {
      close(socketHandle);
    }
}

// destructor for object closes connection
WebClient::~WebClient() {
    close(socketHandle);
}
