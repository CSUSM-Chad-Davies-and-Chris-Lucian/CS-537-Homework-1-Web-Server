//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

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
#include "signal.h"
#include <iostream>

using namespace std;

// class for web server
class WebServer {
public:
    WebServer(string portNumer);
    void StartListening(void (*messageRoutingFunction)(string message, WebServer* webServer, int socketConnection));
    static void *ThreadReadMessage(void *context);
    void WriteMessage(string message, int socketConnection);
    static void CloseConnection(int socketConnection);
    virtual ~WebServer();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif
