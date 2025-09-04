// build: gcc main.c -o debug
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("usage: ./debug <pid>\n");
        return 0;
    }

    pid_t pid = atoi(argv[1]);

    printf("TeenyBug connected @%d",pid);
/*    for(int i = 0; i < 20; i++) {
      kill(pid, SIGUSR1);
      usleep(50000);
    } */
    while(1) {

        printf("\nPress enter to step: ");
        int c = getchar();

        if(c == '\n') {
          kill(pid, SIGUSR1);
        }else {
          while (c != '\n' && c != EOF) {
             c = getchar();
          }
          kill(pid, SIGUSR2);
        }

        usleep(50000);  // change for Windows port (or maybe use wine....)
    }

    return 0;
}
