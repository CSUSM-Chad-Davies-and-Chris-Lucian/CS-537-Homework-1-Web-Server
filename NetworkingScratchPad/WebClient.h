#ifndef WEBCLIENT_H
#define	WEBCLIENT_H

class WebClient {
public:
    WebClient();
    WebClient(const WebClient& orig);
    void Connect(char* ipAddress, int port);
    virtual ~WebClient();
private:

};

#endif	

