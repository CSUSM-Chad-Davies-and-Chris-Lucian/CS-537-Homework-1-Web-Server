#Chris Lucian & Chad Davies
#3/8/2015
#CS 537
#Programming Assignment 1 Web Server

#build server, client, and tests. Exlcudes the clear_ls_make. This occurs whenever just "make" is called
all: server client tests

#build the server
server: server.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o server server.cpp WebServer.cpp WebClient.cpp -pthread

#build the client
client: client.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o client client.cpp WebServer.cpp WebClient.cpp -pthread -lrt -lpthread

#build tests
tests: tests.cpp
	gcc -lstdc++ -o tests tests.cpp

#build the "make clear ls make"
clear_ls_make: clear_ls_make.cpp
	gcc -lstdc++ -o clear_ls_make clear_ls_make.cpp
