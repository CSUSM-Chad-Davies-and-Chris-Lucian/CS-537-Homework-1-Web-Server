//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#ifndef WEBCLIENT_H
#define	WEBCLIENT_H



#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <iostream>

using namespace std;

// web client class for managing socket details
class WebClient {
public:
    WebClient();
    void Connect(string ipAddress, string port, string http_version);
    void ConnectSocket();
    void SendRequest(string request);
    virtual ~WebClient();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
    string version;
};

#endif
