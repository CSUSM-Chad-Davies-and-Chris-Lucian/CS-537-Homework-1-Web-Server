#ifndef WEBSERVER_H
#define	WEBSERVER_H

class WebServer {
public:
    WebServer(char* portNumer);
    WebServer(const WebServer& orig);
    void StartListening();
    virtual ~WebServer();
private:

};

#endif

