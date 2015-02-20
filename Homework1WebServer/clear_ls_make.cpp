#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {



    system("clear");

    system("echo \e[92mKilling Old Processes:\e[0m");
    system("ps | grep server | awk '{print $1}' | xargs kill");
    system("echo \e[92mCurrently Open Processes:\e[0m");
    system("ps");
    system("echo ");
    system("echo \e[92mDirectory Contents:\e[0m");
    system("ls");
    system("echo ");
    system("echo \e[92mMake Results:\e[0m");
    system("make");
    system("echo ");
    return 0;
}
