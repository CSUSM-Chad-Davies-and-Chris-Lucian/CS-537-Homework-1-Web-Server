#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    string port1;
    port1 = argv[1];

    system("echo \e[92mKilling Old Processes:\e[0m");
    system("ps | grep server | awk '{print $1}' | xargs kill");
    system("ps | grep client | awk '{print $1}' | xargs kill");

    printf("getlogin_r %s", getlogin());

    printf("\nTESTS: Starting Server App\n");
    string command = "./server " + port1 + " &";

    system(command.c_str());

    sleep(1);

    for (size_t index = 0; index < 1; index++) {
        printf("\nTESTS: Starting Client App %d\n",index);

        string command2 = "./client localhost " + port1 + " &";

        system(command2.c_str());
    }

    return 0;
}
