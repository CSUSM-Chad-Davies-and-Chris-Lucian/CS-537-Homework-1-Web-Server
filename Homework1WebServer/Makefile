all: server client tests

server: server.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o server server.cpp WebServer.cpp WebClient.cpp -pthread

client: client.cpp WebServer.cpp WebClient.cpp
	gcc -lstdc++ -o client client.cpp WebServer.cpp WebClient.cpp -pthread

tests: tests.cpp
	gcc -lstdc++ -o tests tests.cpp
