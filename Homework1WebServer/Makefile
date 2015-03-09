all: server client tests

server: server.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o server server.cpp WebServer.cpp WebClient.cpp -pthread

client: client.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o client client.cpp WebServer.cpp WebClient.cpp -pthread -lrt -lpthread

tests: tests.cpp
	gcc -lstdc++ -o tests tests.cpp

clear_ls_make: clear_ls_make.cpp
	gcc -lstdc++ -o clear_ls_make clear_ls_make.cpp
