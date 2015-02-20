#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

class WebServer {
public:
    WebServer(string portNumer);
    void StartListening(void (*messageRoutingFunction)(string message));
    static void *ReadMessage(int sockentConnection,void (*messageRoutingFunction)(string message));
    static void *ThreadReadMessage(void *context);
    virtual ~WebServer();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif
