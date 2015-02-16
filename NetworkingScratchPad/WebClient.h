#ifndef WEBCLIENT_H
#define	WEBCLIENT_H

class WebClient {
public:
    WebClient();
    WebClient(const WebClient& orig);
    void Connect(char* ipAddress, char* port);
    void SendGetRequestAndAwaitResponse();
    virtual ~WebClient();
private:

};

#endif	

