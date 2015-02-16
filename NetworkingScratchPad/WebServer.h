#ifndef WEBSERVER_H
#define	WEBSERVER_H

#include<sys/socket.h>
#include<netinet/in.h>

class WebServer {
public:
    WebServer(char* portNumer);
    WebServer(const WebServer& orig);
    void StartListening();
    virtual ~WebServer();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif

