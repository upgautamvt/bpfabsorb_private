#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Started process with pid: %u\n", getpid());
    char buf[256];
    getcwd(buf, 256);
    return 0;
}
