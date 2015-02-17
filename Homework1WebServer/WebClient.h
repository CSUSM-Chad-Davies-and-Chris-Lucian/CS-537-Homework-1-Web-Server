#ifndef WEBCLIENT_H
#define	WEBCLIENT_H

#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <unistd.h>
#include <string>
using namespace std;

class WebClient {
public:
    WebClient();
    WebClient(const WebClient& orig);
    void Connect(string ipAddress, string port);
    void SendGetRequestAndAwaitResponse();
    virtual ~WebClient();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

