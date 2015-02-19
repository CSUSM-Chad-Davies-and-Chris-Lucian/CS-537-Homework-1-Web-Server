#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("\nTESTS: Starting Server App\n");
    system("./server 82341 &");

    sleep(2);

    printf("\nTESTS: Starting Client App\n");
    system("./client 127.0.0.1 82341 &");

    return 0;
}
