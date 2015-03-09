//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>

using namespace std;

// this program automates the testing of the two applications with a single
// argument of a port
int main(int argc, char *argv[]) {
    string port1;
    port1 = argv[1];

    // kill all clients and servers before executing a test
    system("echo \e[92mKilling Old Processes:\e[0m");
    system("ps | grep server | awk '{print $1}' | xargs kill");
    system("ps | grep client | awk '{print $1}' | xargs kill");

    // construct the server command and start the test
    printf("\nTESTS: Starting Server App\n");
    string command = "./server " + port1 + " &";
    system(command.c_str());

    // wait for server to initialize and begin listening
    sleep(1);

    // create clients in for loop for running tests
    for (size_t index = 0; index < 1; index++) {
        printf("\nTESTS: Starting Client App %d\n",index);

        string command2 = "./client localhost " + port1 + " &";

        system(command2.c_str());
    }

    return 0;
}
