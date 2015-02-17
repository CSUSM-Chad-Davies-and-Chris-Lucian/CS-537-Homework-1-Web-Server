#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <string>
using namespace std;

class WebServer {
public:
    WebServer(string portNumer);
    void StartListening();
    virtual ~WebServer();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif

