#include "WebServer.h"

struct readMessageParams{
  int socketHandle;
  int socketConnection;
  void(*messageRoutingFunction)(string message, WebServer *server, int socketConnection);
  WebServer* webServer;
};


WebServer::WebServer(string portNumer) {

    socketHandle = socket(AF_INET, SOCK_STREAM, 0);

    if(socketHandle < 0)
    {
        fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
              errno, strerror(errno));
        return;
    }

    bzero((char *) &socketInfo, sizeof(socketInfo));
    int portInt = atoi(portNumer.c_str());
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    socketInfo.sin_port = htons(portInt);

    int binding = bind(socketHandle,(struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in));
    if(binding < 0)
    {
        fprintf(stderr, "Error bind to socket, erron = %d (%s) \n",
                errno, strerror(errno));
        return;
    }

    listen(socketHandle,100);
}

void WebServer::StartListening(void (*messageRoutingFunction)(string message, WebServer* webServer, int socketConnection)){
    if(socketHandle >= 0)
    {
        pthread_t thread_id[10];
        int threadIncrement = 0;
        bool startJoining = false;
        while(1)
        {
            socklen_t client_length;
            struct sockaddr_in client_address;
            client_length = sizeof(client_address);
            cout << "41" << endl;
            int socketConnection = accept(socketHandle,
              (struct sockaddr *) &client_address, &client_length);
            cout << "42" << endl;

            //printf ("=================================socketconnection: %d", socketConnection);
            cout << "43" << endl;
            if(socketConnection < 0)
            {
              cout << "44" << endl;
                sleep(2);
                //cout << "SERVER Socket connection less than 0" << endl;
                //fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                //        errno, strerror(errno));
                //close(socketHandle);
                //return;
            }
            else
            {
              cout << "45" << endl;
              struct readMessageParams params;
              params.socketHandle = socketHandle;
              params.socketConnection = socketConnection;
              params.messageRoutingFunction = messageRoutingFunction;
              params.webServer = this;
              cout << "46" << endl;

              if(startJoining)
              {
                pthread_join(thread_id[threadIncrement], NULL);
              }

              int x = pthread_create(&thread_id[threadIncrement], NULL, &ThreadReadMessage, (void *)&params);

              if(threadIncrement == 9)
              {
                startJoining = true;
              }

              threadIncrement = (threadIncrement+1) % 10;

              cout << "x: " << x << endl;
              cout << "47" << endl;
            }
            cout << "48" << endl;
        }
    }
}

void *WebServer::ThreadReadMessage(void *context)
{
  cout << "49" << endl;
  while(1)
  {
    struct readMessageParams *params = ((struct readMessageParams *)context);
    int socketConnection =params->socketConnection;
    int socketHandle  = params->socketHandle;

    int failedWhenNegative;
    char  buffer[4000];
    bzero(buffer,4000);
    cout << "50" << endl;
    failedWhenNegative = read(socketConnection,buffer,4000);
    cout << "51" << endl;
    if (failedWhenNegative < 0) {
        printf ("\nreadfailed %d", socketConnection);
        CloseConnection(socketConnection);
        return  NULL;
    }

    string message = string(buffer);
    cout << "52" << endl;
    params->messageRoutingFunction(message, params->webServer, socketConnection);
  }
  cout << "53" << endl;
}

void WebServer::WriteMessage(string message, int socketConnection)
{
  int length = message.length();
  char  buffer[length];

  bzero(buffer,length);
  strcpy(buffer, message.c_str());
  int failedWhenNegative = write(socketConnection,buffer,length);

  if (failedWhenNegative < 0)  {
      printf("Server Write Failed");
      fprintf(stderr, "Error writing to socket, errno = %d (%s)\n",
              errno, strerror(errno));
      printf ("\nwritefailed %d", socketConnection);
      CloseConnection(socketConnection);
      return;
  }
}

void WebServer::CloseConnection(int socketConnection)
{
  cout << "calling it!!!" << endl;
  printf("\n\n\n\n\nclosed#########%d##################\n\n\n\n\n", socketConnection);


  shutdown(socketConnection, SHUT_RDWR);

  char buffer[200];
  while(read(socketConnection, buffer, 200) > 0);
  close(socketConnection);
  cout << "before pthread die" << endl;
  pthread_exit(NULL);
  cout << "after pthread die" << endl;
}

WebServer::~WebServer() {
}
