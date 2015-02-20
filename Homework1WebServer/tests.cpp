#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("\nTESTS: Starting Server App\n");
    system("./server 82341 &");

    //sleep(2);

    for (size_t index = 0; index < 3; index++) {
        printf("\nTESTS: Starting Client App %d\n",index);
        system("./client 127.0.0.1 82341 &");
    }


    return 0;
}
