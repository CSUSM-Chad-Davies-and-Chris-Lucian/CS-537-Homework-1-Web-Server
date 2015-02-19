all: server client tests

server: server.cpp WebServer.cpp WebClient.cpp
	g++ -o server server.cpp WebServer.cpp WebClient.cpp -pthread

client: client.cpp WebServer.cpp WebClient.cpp
	g++ -o client client.cpp WebServer.cpp WebClient.cpp -pthread

tests: main.cpp
	g++ -o tests main.cpp
