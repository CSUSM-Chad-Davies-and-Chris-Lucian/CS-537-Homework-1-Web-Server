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
    cout << "21" << endl;
    strcpy(buffer, request.c_str());
    int failedWhenNegative = write(socketHandle, buffer, request.length());

    cout << "22" << endl;
    if (failedWhenNegative < 0)  {
      cout << "23" << endl;
        printf("Client Read Failed");
        fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                errno, strerror(errno));
        return;
    }
    cout << "24" << endl;
    string result;
    int bytes_read = 0;
    int total_to_read = size;

    cout << "24" << endl;
    //do
    //{
      bzero(buffer,size);
      cout << "25" << endl;
      printf("beging do while");
      cout << "26" << endl;
      bytes_read = read(socketHandle,buffer,total_to_read);
      cout << "27" << endl;
      if (bytes_read < 0)  {
        cout << "28" << endl;
          printf("Client Write Failed");
          fprintf(stderr, "Error reading from socket, errno = %d (%s) \n",
                  errno, strerror(errno));
          return;
      }
      cout << "29" << endl;
      total_to_read -= bytes_read;
      printf("end do while %s", result.c_str());
      result = result + string(buffer);
      cout << "30" << endl;

      if(result.find("Content-Length:") != std::string::npos)
      {
        cout << "31" << endl;
        printf ("find if worked");
      }
      else
      {
        cout << "32" << endl;
        printf ("find if didn't work");
      }

    //}while(result.find("Content-Length:") != std::string::npos);

    cout << "33" << endl;
    printf("%s\n",result.c_str());

    cout << "34" << endl;
    if(version == "1.0")
    {
      cout << "36" << endl;
      close(socketHandle);
    }
    cout << "35" << endl;
}

WebClient::~WebClient() {
    close(socketHandle);
}
