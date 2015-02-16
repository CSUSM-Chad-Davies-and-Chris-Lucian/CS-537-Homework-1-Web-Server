#ifndef WEBCLIENT_H
#define	WEBCLIENT_H

#include<sys/socket.h>
#include<netinet/in.h>
#include <string.h>

class WebClient {
public:
    WebClient();
    WebClient(const WebClient& orig);
    void Connect(char* ipAddress, char* port);
    void SendGetRequestAndAwaitResponse();
    virtual ~WebClient();
private:
    int socketHandle;
    struct sockaddr_in socketInfo;
};

#endif	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

