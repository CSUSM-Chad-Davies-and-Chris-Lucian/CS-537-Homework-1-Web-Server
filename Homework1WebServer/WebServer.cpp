//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#include "WebServer.h"

// struct for parameters for threaded function call
struct readMessageParams{
  int socketHandle;
  int socketConnection;
  void(*messageRoutingFunction)(string message, WebServer *server, int socketConnection);
  WebServer* webServer;
};

// constructor for web server initializes socket and listens for requests
// from new clients
WebServer::WebServer(string portNumer) {

  signal(SIGPIPE, SIG_IGN);

  // creates the socket
    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    // close on error
    if(socketHandle < 0)
    {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
              errno, strerror(errno));
        return;
    }

    // clears and initializes the socket info
    bzero((char *) &socketInfo, sizeof(socketInfo));
    int portInt = atoi(portNumer.c_str());
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    socketInfo.sin_port = htons(portInt);

    // binds the socket
    int binding = bind(socketHandle,(struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in));

    // closes on error
    if(binding < 0)
    {
        fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    // liseten for the connections
    listen(socketHandle,100);
}

// start listening - creates an infinite while loop for connections
// routes them to the message routing fuction
void WebServer::StartListening(void (*messageRoutingFunction)(string message, WebServer* webServer, int socketConnection)){
    if(socketHandle >= 0)
    {
        // declares array of 10 possible threads
        // limited the number of threads to 10 due to limitations on empress
        pthread_t thread_id[10];
        int threadIncrement = 0;
        bool startJoining = false;

        // infinite loop to accept unlimited number of connections
        while(1)
        {
          // accepts a new connection
          socklen_t client_length;
          struct sockaddr_in client_address;
          client_length = sizeof(client_address);
          int socketConnection = accept(socketHandle,
            (struct sockaddr *) &client_address, &client_length);

          // if there is an error sleep then try to accept new connection
          if(socketConnection < 0)
          {
              sleep(2);
          }
          else
          {
            // if no error read message parameters
            struct readMessageParams params;
            params.socketHandle = socketHandle;
            params.socketConnection = socketConnection;
            params.messageRoutingFunction = messageRoutingFunction;
            params.webServer = this;

            // if reached max num of threads begin joining threads before
            // creating new ones
            if(startJoining)
            {
              pthread_join(thread_id[threadIncrement], NULL);
            }

            // create a new thread and pass the params to the new thread
            int x = pthread_create(&thread_id[threadIncrement], NULL, &ThreadReadMessage, (void *)&params);

            // detect when reached max num of threads and begin joining
            if(threadIncrement == 9)
            {
              startJoining = true;
            }

            // increment thread index
            threadIncrement = (threadIncrement+1) % 10;
          }
        }
    }
}

// thread for reading messages from the client
// this is done in infinte loop to accomodate http 1.1 requests
void *WebServer::ThreadReadMessage(void *context)
{
  // infinte while loop for http 1.1 requests
  while(1)
  {
    // extract read message params
    struct readMessageParams *params = ((struct readMessageParams *)context);
    int socketConnection =params->socketConnection;
    int socketHandle  = params->socketHandle;

    // read message from socket
    int failedWhenNegative;
    char  buffer[4000];
    bzero(buffer,4000);
    failedWhenNegative = read(socketConnection,buffer,4000);

    // close when failed
    if (failedWhenNegative < 0)
    {
        CloseConnection(socketConnection);
        return  NULL;
    }

    // convert message to string and pass string to message router
    string message = string(buffer);
    params->messageRoutingFunction(message, params->webServer, socketConnection);
  }
}

// given a string writes message to given socket
void WebServer::WriteMessage(string message, int socketConnection)
{
  // write message to socket
  int length = message.length();
  char  buffer[length];
  bzero(buffer,length);
  strcpy(buffer, message.c_str());
    int failedWhenNegative = write(socketConnection,buffer,length);

  // close at fail
  if (failedWhenNegative < 0)  {
      printf("Server Write Failed");
      fprintf(stderr, "Error writing to socket, errno = %d (%s)\n",
              errno, strerror(errno));
      printf ("\nwritefailed %d", socketConnection);
      CloseConnection(socketConnection);
      return;
  }
}

// close connection properly by reading the remaining info out of the buffer
// after shutdown before close then exit the thread
void WebServer::CloseConnection(int socketConnection)
{
  shutdown(socketConnection, SHUT_RDWR);

  char buffer[200];
  while(read(socketConnection, buffer, 200) > 0);
  close(socketConnection);
  pthread_exit(NULL);
}

// empty destructor due to infinte loop
WebServer::~WebServer() {
}
