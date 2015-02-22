#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    system("echo \e[92mKilling Old Processes:\e[0m");
    system("ps | grep server | awk '{print $1}' | xargs kill");
    system("ps | grep client | awk '{print $1}' | xargs kill");

    printf("\nTESTS: Starting Server App\n");
    system("./server 82341 &");

    for (size_t index = 0; index < 3; index++) {
        printf("\nTESTS: Starting Client App %d\n",index);
        system("./client 127.0.0.1 82341 &");
    }

    return 0;
}
