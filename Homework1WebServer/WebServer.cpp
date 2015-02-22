#include "WebServer.h"

struct readMessageParams{
  int socketConnection;
  void(*messageRoutingFunction)(string message, WebServer *server);
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

    listen(socketHandle,1);
}

void WebServer::StartListening(void (*messageRoutingFunction)(string message, WebServer* webServer)){
    if(socketHandle >= 0)
    {
        pthread_t thread_id;
        while(1)
        {
            socklen_t client_length;
            struct sockaddr_in client_address;
            client_length = sizeof(client_address);
            int socketConnection = accept(socketHandle,
              (struct sockaddr *) &client_address, &client_length);
            if(socketConnection < 0)
            {
                fprintf(stderr, "Error accepting socket connection request, errno = %d (%s) \n",
                        errno, strerror(errno));
                return;
            }

            struct readMessageParams params;
            params.socketConnection = socketConnection;
            params.messageRoutingFunction = messageRoutingFunction;
            params.webServer = this;
            pthread_create(&thread_id, NULL, &ThreadReadMessage, (void *)&params);
        }
        close(socketHandle);
    }
}


void *WebServer::ThreadReadMessage(void *context)
{
  struct readMessageParams *params = ((struct readMessageParams *)context);
  int socketConnection =params->socketConnection;

  int failedWhenNegative;
  int sockfd;
  char  buffer[256];
  bzero(buffer,256);
  failedWhenNegative = read(socketConnection,buffer,255);
  if (failedWhenNegative < 0) {
      fprintf(stderr, "Error reading from socket, errno = %d (%s)\n",
              errno, strerror(errno));
      close(sockfd);
      return  NULL;
  }

  string message = string(buffer);
  params->messageRoutingFunction(message, params->webServer);
  return  NULL;
}

WebServer::~WebServer() {
    close(socketHandle);
}
