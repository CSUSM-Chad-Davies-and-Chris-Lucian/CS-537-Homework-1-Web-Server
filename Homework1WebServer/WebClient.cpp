#include "WebClient.h"
WebClient::WebClient() {
}

void WebClient::Connect(string ipAddress, string port, string http_version){

    version = http_version;

    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int portInt = atoi(port.c_str());
    socketInfo.sin_port = htons(portInt);

    if(http_version == "1.1")
    {
      ConnectSocket();
    }
}

void WebClient::ConnectSocket()
{
  socketHandle = socket(AF_INET, SOCK_STREAM, 0);

  if(socketHandle < 0)
  {
      fprintf(stderr, "Error opening socket, errno = %d (%s) \n",
              errno, strerror(errno));
              close(socketHandle);
      return;
  }


  int connectionSuccess = connect(socketHandle, (struct sockaddr *) &socketInfo, sizeof (sockaddr_in));
  if(connectionSuccess < 0)
  {
      fprintf(stderr, "Error connecting to socket, errno = %d (%s) \n",
              errno, strerror(errno));
              close(socketHandle);
      return;
  }
}

void WebClient::SendRequest(string request){
  if(version == "1.0")
  {
    ConnectSocket();
  }

    int rc = 0; // Actual number of bytes read by function read()
    int size = 100000;
    char buffer[size];
    bzero(buffer,size);
    printf("Client Sending Request %s\n", request.c_str());

    strcpy(buffer, request.c_str());
    int failedWhenNegative = write(socketHandle, buffer, request.length());

    if (failedWhenNegative < 0)
    {

        printf("Client Read Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }


    string result;
    int bytes_read = 0;
    int total_to_read = size;

    bzero(buffer,size);
    bytes_read = read(socketHandle,buffer,total_to_read);

    if (bytes_read < 0)
    {
        printf("Client Write Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }
    total_to_read -= bytes_read;
    result = result + string(buffer);

    if(version == "1.0")
    {
      close(socketHandle);
    }
}

WebClient::~WebClient() {
    close(socketHandle);
}
