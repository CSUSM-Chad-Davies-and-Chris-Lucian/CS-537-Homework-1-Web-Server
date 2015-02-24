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


class WebClient {
public:
    WebClient();
    void Connect(string ipAddress, string port);
    void SendGetRequestAndAwaitResponse();
    virtual ~WebClient();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif
