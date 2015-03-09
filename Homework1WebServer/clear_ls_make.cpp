//Chris Lucian & Chad Davies
//3/8/2015
//CS 537
//Programming Assignment 1 Web Server

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

//This main file is used to help quickly itterate on development when trying to build and clean up the programs in the project
int main(int argc, char *argv[]) {
    //Use the system command to clear the console
    system("clear");

    //Use the system commands to kill all server and client processes
    system("echo \e[92mKilling Old Processes:\e[0m");
    system("ps | grep server | awk '{print $1}' | xargs kill");
    system("ps | grep client | awk '{print $1}' | xargs kill");

    //Use the system command to display currently open processes
    system("echo \e[92mCurrently Open Processes:\e[0m");
    system("ps");
    system("echo ");

    //Use the system command to display the current directory contents
    system("echo \e[92mDirectory Contents:\e[0m");
    system("ls");
    system("echo ");

    //Use the system command to call make for the project
    system("echo \e[92mMake Results:\e[0m");
    system("make");
    system("echo ");
    return 0;
}
